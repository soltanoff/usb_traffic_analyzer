/* =====================================================================================================================
 * File: client.cpp
 * Description: Реализация работы пользователя с сервером
 * Created: 01.02.2016
 * Author: soltanoff
 * ================================================================================================================== */
#include "client.h"
#include "../core/commands/scheduler.h"
#include <cstring>
#include <regex>


CClient::CClient() {
#ifdef _WIN32
    WSADATA wsa_data; // содержит информацию о реализации сокетов Windows
    int wsa_result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    // MAKEWORD(2,2) данной функции запрашивает версию WinSock системы и
    // устанавливает ее как наивысшую допустимую версию сокетов Windows,
    // котора¤ может использоваться.

    if (wsa_result != NO_ERROR)
    {
        std::cout << "[ERROR: WSADATA] Error at WSAStartup()\n";
        WSACleanup();
        system("pause");
        return;
    }
#endif
    have_ip = false;
    m_socket = socket(AF_INET, SOCK_STREAM, ServerCfg::PROTOCOL);

    if (m_socket < 0)
    {
        std::cout << "[ERROR: SOCKET] Error at socket()" << std::endl;
        system("pause");
        return;
    }
    std::cout << "[STATUS] Client ready.\n";
}

CClient::~CClient() {
    close();
}

bool CClient::send_message(const char* msg) {
    send(m_socket, msg, strlen(msg), 0);
    return true;
}

void CClient::send_command() {
    static std::regex re("(\\w+)\\s*(.*)?\\s*");
    std::string sendbuf;
    std::cout << "[CLIENT] Command: ";

    std::cin.clear();
    std::cin.sync();
    std::getline(std::cin, sendbuf);

    if (sendbuf.compare("exit") == 0) {
        throw 0;
    }

    std::smatch result;
    if (std::regex_search(sendbuf, result, re)) {
        std::vector<std::uint8_t> data;
        std::string cmd(result.str(1));
        std::string args = result.str(2);
        // ==========================================
        // Command selector
        COMMANDS cmd_code;
        if (cmd == "help")
            cmd_code = COMMANDS::help;
        else if (cmd == "test")
            cmd_code = COMMANDS::test;
        else if (cmd == "rr")
            cmd_code = COMMANDS::rr;
        else if (cmd == "get_buf")
            cmd_code = COMMANDS::get_buf;
        else if (cmd == "device")
            cmd_code = COMMANDS::device;
        else if (cmd == "command")
            cmd_code = COMMANDS::command;
        else if (cmd == "store")
            cmd_code = COMMANDS::store;
        else if (cmd == "get_all")
            cmd_code = COMMANDS::get_all;
        else if (cmd == "get_read")
            cmd_code = COMMANDS::get_read;
        else if (cmd == "get_unread")
            cmd_code = COMMANDS::get_unread;
        else if (cmd == "set_read_all")
            cmd_code = COMMANDS::set_read_all;
        else if (cmd == "clear_db")
            cmd_code = COMMANDS::clear_db;
        else
            cmd_code = COMMANDS::unknown;

        sendbuf = (std::uint32_t)cmd_code > 9 ? std::to_string((std::uint32_t)cmd_code) : "0" + std::to_string((std::uint32_t)cmd_code);
        sendbuf += "%" + args + '\0';
        int bytesSent = send_message(sendbuf.c_str());
        if (bytesSent < 0)
            throw 0;
    }
}

bool CClient::get_answer(int& bytesRecv, char* answer) {
    bytesRecv = (int)recv(m_socket, answer, (size_t) ServerCfg::BUFF_SIZE, 0); // MSG_WAITALL)
    if (bytesRecv >= 1024) {
        char recvbuf[ServerCfg::BUFF_SIZE] = "";
        while(recv(m_socket, recvbuf, (size_t) ServerCfg::BUFF_SIZE, MSG_DONTWAIT) >= 1024) {
            strcat(answer, recvbuf);
            strcpy(recvbuf, "");
        }
    }

    if (bytesRecv == 0) {
        std::cout << "[CLIENT] Connection closed.\n";
        throw 0;
    }
    if (bytesRecv < 0) {
        std::cout << "[CLIENT] Connection lost.\n";
        throw 0;
    }
    std::cout << "BYTESRECV: " << bytesRecv << std::endl;
    return true;
}

void CClient::answer_control(char* answer) {
    while (true) {
        std::cin.getline(answer, 2, '\n');
        if ((strlen(answer) > 1 ||
             (answer[0] != 'Y' && answer[0] != 'N' && answer[0] != 'y' && answer[0] != 'n'))) {
            std::cout << "Error, try again: [Y/N] ";
            continue;
        } else break;
    }
}

void CClient::ipaddres_control(char* ip) {
    while (true) {
        std::cin.getline(ip, 17, '\n');
        if (inet_addr(ip) == INADDR_NONE) {
            std::cout << "Error, try again: ";
            continue;
        } else break;
    }
}

int CClient::try_open_socket() {
    char answear[128];
    char ipaddres[16];

    if (!have_ip) {
        std::cout << "[CLIENT] Do you want to enter server IP address? [Y/N] ";
        answer_control(answear);
        if (answear[0] == 'N' || answear[0] == 'n') {
            strcpy(ipaddres, ServerCfg::LOCALHOST);
            std::cout << "[CLIENT] Connecting to server...\n";
        } else {
            std::cout << "[CLIENT] Enter server IP addres: ";
            ipaddres_control(ipaddres);
            std::cout << "[CLIENT] Try to connect to " << ipaddres << "...\n";
        }
        have_ip = true;
    }
    // service содержит информаци¤ о семействе адресов,
    // IP адрес и номер порта
    service.sin_family = AF_INET; // семейство адресов »нтернет
    service.sin_addr.s_addr = inet_addr(ipaddres); // локальный IP
    service.sin_port = htons(ServerCfg::PORT); // номер порта

    if (connect(m_socket, (sockaddr *)& service, sizeof(service)) == SOCKET_ERROR) {
        std::cout << "[ERROR: sockaddr] Connection failed.\n";
        return -1;
    }

    send_message("1");  // send client type
    return 0;
}

int CClient::exec() {
    int bytesRecv = SOCKET_ERROR;
    // char recvbuf[ServerCfg::BUFF_SIZE] = "";

    while (true) {
        char recvbuf[ServerCfg::BUFF_SIZE] = "";
        try {
            get_answer(bytesRecv, recvbuf);
            // std::cout << "[SERVER] Bytes recv: " << bytesRecv << " > \n" << recvbuf << "\n";
            std::cout << "[SERVER] > " << recvbuf << "\n";
            send_command();
        }
        catch (int){
            std::cout << "[CLIENT] Terminate.\n";
            close();
            return 0;
        }
        catch (...){
            std::cout << "[CLIENT] Client get error in CClient::exec().\n";
            close();
            return 0;
        }
    }
}

void CClient::start() {
    std::cout << "[STATUS] Client started.\n";
    std::cout << "[CLIENT] Try to connect...\n";

    if (try_open_socket() >= 0)
        exec();
}

void CClient::close() {
    shutdown(m_socket, 1);
}
