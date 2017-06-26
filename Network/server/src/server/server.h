/* =====================================================================================================================
 * File: server.h
 * Description: Реализация работы сервера
 * Created: 01.02.2017
 * Author: soltanoff
 * ================================================================================================================== */
#ifndef SERVER_H
#define SERVER_H
/* ================================================================================================================== */
#include "../core/clientinteraction.h"
#include "../core/connection.h"
/* ================================================================================================================== */
//! вектор сокетов подключенных к серверу GSM-устройств
extern std::vector<SOCKET> GSM_MODULES_SOCKETS;
//! @typedef тип подключенного клиента
enum class CLIENT_TYPES : std::uint32_t {
    simple=1,
    gsm
};
/*!
 * @class CServer
 * @inherit IBaseConnection
 * Класс осуществляющий работу сервера
 */
class CServer: public IBaseConnection {
public:
    CServer();
    ~CServer();
    /*!
     * @public Метод осуществляющий инициализацию сокета и дальнейшего подключения к серверу.
     * @return 0 если открыть сокет получилось, иначе -1
     * @note Содержит в себе логику настроки сетевого подключения
     */
    int try_open_socket();
    /*!
     * @public Основной циклический метод класса.
     * @return 0 если работа завершилась без аварий, иначе -1
     * @note основной метод для взаимодействия сервера с клиентами
     */
    int exec();
    /*!
     * @public Метод позволяющий начать работу серверной части.
     * @return None
     * @note запуск методов CClient::try_open_socket(), CClient::exec()
     */
    void start();
    /*!
     * @public Метод закрывающий сокет сервера.
     * @return None
     * @note закрытие сокета с параметром SHUT_WR
     */
    void close();
private:
    //! @private вектор умных указателей на handler'ы потоков клиентов
    std::vector<std::shared_ptr<std::thread>> _client_threads;
    //! @private вектор активных сокетов клиентов
    std::vector<SOCKET> _client_sockets;
    //! @private вектор ip-адресов клиентов
    std::vector<char*> _client_ips;
    /*!
     * @private Метод определяющий является ли подключенный клиент устройством GSM.
     * @param AcceptSocket - сокет подключенного клиента
     * @param client_number - номер подключенного клиента
     * @return true - подключилось устройство GSM, иначе false
     * @note осуществляет потверждение подключения клиента к серверу (отправляет клиенту статус подключения)
     * @note определяет тип подключенного клиента
     */
    bool is_gsm_client(const SOCKET& AcceptSocket, const int& client_number);
    /*!
     * @private Метод осуществляющий подключение клиента к серверу на уровне сокетов.
     * @param AcceptSocket - сокет подключенного клиента
     * @param ClientInfo - структура содержащая основную информацию о подключенном клиенте
     * @return None
     * @note осуществляет потверждение подключения клиента к серверу на уровне сокетов
     */
    void accept_socket(SOCKET& AcceptSocket, sockaddr_in& ClientInfo);
    /*!
     * @private Метод регистрирующий подключение клиента .
     * @param AcceptSocket - сокет подключенного клиента
     * @param ClientInfo - структура содержащая основную информацию о подключенном клиенте
     * @param count - значение счетчика клиентов
     * @return None
     * @note осуществляется регистрация: сокета, ip-адреса клиента, handler'а потока обработки данных клиента
    */
    void connect_user(const SOCKET& AcceptSocket, const sockaddr_in& ClientInfo, const std::uint32_t& count);
};
/* ================================================================================================================== */
#endif /* SERVER_H */
