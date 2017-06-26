/* =====================================================================================================================
 * File: server.h
 * Description: Реализация работы сервера
 * Created: 01.02.2017
 * Author: soltanoff
 * ================================================================================================================== */
#include "server.h"
std::vector<SOCKET> GSM_MODULES_SOCKETS;

void thread_routine(std::uint32_t client_number, SOCKET client_socket, bool is_gsm) {
    ClientInteraction client(client_number, client_socket, is_gsm);

    while (true) {
        try {
            int code = client.exec();
            if (code == -1) {
                std::cout << "[SERVER] Client #" << client_number + 1 << " disconnected.\n";
                client.close();
                return;
            }
        }
        catch (int) {
            client.close();
            return;
        }
        catch (...) {
            std::cout << "[ERROR] Server get error in thread_routine().\n";
            client.close();
            return;
        }
    }
}

CServer::CServer() {
    m_socket = socket(AF_INET, SOCK_STREAM, ServerCfg::PROTOCOL);
    // в качестве параметров используются семейство интернет-адресов (IP),
    // потоковые сокеты и протокол TCP/IP.

    if (m_socket < 0)
    {
        std::cout << "[ERROR: SOCKET] Error at socket()" << std::endl;
        return;
    }

    std::cout << "[STATUS] Server ready.\n";
}

CServer::~CServer() {
    close();
}

void CServer::accept_socket(SOCKET& AcceptSocket, sockaddr_in& ClientInfo) {
    int adrlen = sizeof(ClientInfo);
    AcceptSocket = (SOCKET) SOCKET_ERROR;

    while (AcceptSocket == SOCKET_ERROR)
        AcceptSocket = accept(m_socket, (sockaddr* )& ClientInfo, (socklen_t *) &adrlen);
}

bool CServer::is_gsm_client(const SOCKET& AcceptSocket, const int& client_number) {
    std::int32_t bytesRecv;
    char recvbuf[ServerCfg::BUFF_SIZE] = "";

    send(AcceptSocket, "ACCEPT", strlen("ACCEPT"), 0);
    bytesRecv = (std::int32_t)recv(AcceptSocket, recvbuf, ServerCfg::BUFF_SIZE, 0);

    if (bytesRecv == 0) {
        std::cout << "[SERVER] Client #" << client_number + 1 << " disconnected.\n";
        throw 0;
    }
    if (bytesRecv < 0) {
        std::cout << "[SERVER] Connection lost with client #" << client_number + 1 << std::endl;
        throw 0;
    }

    try {
        std::uint32_t id = (std::uint32_t) std::stoi(recvbuf);
        switch (id){
            case (std::uint32_t) CLIENT_TYPES::gsm:
                GSM_MODULES_SOCKETS.push_back(AcceptSocket);
                return true;
            case (std::uint32_t) CLIENT_TYPES::simple:
                return false;
            default:
                return false;
        }
    }
    catch (...) {
        return false;
    }
}

void CServer::connect_user(const SOCKET& AcceptSocket, const sockaddr_in& ClientInfo, const std::uint32_t& count) {
    try {
        std::cout << "[SERVER] Client #"
                  << count + 1
                  << " connected. "
                  << inet_ntoa(ClientInfo.sin_addr) << std::endl;

        // std::thread* thread = new std::thread(thread_routine, count, AcceptSocket);
        std::shared_ptr<std::thread> thread(
                new std::thread(thread_routine, count, AcceptSocket, is_gsm_client(AcceptSocket, count))
        );

        _client_ips.push_back(inet_ntoa(ClientInfo.sin_addr));
        _client_sockets.push_back(AcceptSocket);
        _client_threads.push_back(thread);
    }
    catch (std::exception e) {
        std::cout << "[ERROR] CServer::connect_user(): " << e.what() << std::endl;
    }
    catch (...) {
        std::cout << "[ERROR] Server get error in CServer::connect_user().\n";
    }
}

int CServer::try_open_socket() {
    // service содержит информация о семействе адресов,
    // IP адрес и номер порта
    service.sin_family = AF_INET; // семейство адресов »нтернет
    service.sin_addr.s_addr = INADDR_ANY;
    service.sin_port = htons(ServerCfg::PORT); // номер порта

    if (bind(m_socket, (sockaddr*)& service, sizeof(service)) == SOCKET_ERROR)
    {
        std::cout << "[ERROR: sockaddr] bind() failed.\n";
        return -1;
    }
    /* ========================================================================================================== */
    if (listen(m_socket, ServerCfg::BACKLOG) == SOCKET_ERROR)
    {
        std::cout << "[ERROR: LISTEN] Error listening on socket.\n";
        return -1;
    }
    return 0;
}

int CServer::exec() {
    std::uint32_t count = 0;

    while (true)
    {
        try {
            SOCKET AcceptSocket;
            sockaddr_in ClientInfo;

            accept_socket(AcceptSocket, ClientInfo);

            connect_user(AcceptSocket, ClientInfo, count);
            count++;
        }
        catch (std::exception e) {
            std::cout << "[ERROR] CServer::exec(): " << e.what() << std::endl;
            close();
            return 0;
        }
        catch (...){
            std::cout << "[ERROR] Server get error in CServer::exec().\n";
            close();
            return 0;
        }
    }
}

void CServer::start() {
    std::cout << "[STATUS] Server started.\n";
    std::cout << "[SERVER] Waiting for a client to connect...\n";

    if (try_open_socket() >= 0)
        exec();
}

void CServer::close() {
    shutdown(m_socket, 1);
}