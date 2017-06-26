/* =====================================================================================================================
 * File: database.h
 * Description: Подключение к базе данных MySQL сервера
 * Created: 17.02.2017
 * Author: soltanoff
 * ================================================================================================================== */
#ifndef DATABASE_H
#define DATABASE_H
/* ================================================================================================================== */
#include <mysql/mysql.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
/* ================================================================================================================== */
//! @typedef вектор результатов выполнения запроса
typedef std::vector<std::string> QueryResult;
//! @typedef список (вектор) векторов результатов выполнения запроса
typedef std::vector<std::vector<std::string>> QueryResultList;
/* ================================================================================================================== */
/*!
 * @class CDBConnection
 * Класс осуществляющий подключение к БД MySQL
 */
class CDBConnection {
public:
    CDBConnection() {};
    CDBConnection(
            const std::string& server,
            const std::string& database,
            const std::string& user,
            const std::string& password,
            const unsigned int port = 0
    );
    ~CDBConnection() { close(); };
    /*!
     * @public Метод осуществляющий подключение к БД MySQL
     * @param server - ip-адрес сервера БД
     * @param database - имя БД
     * @param user - логин пользователя БД
     * @param password - пароль пользователя БД
     * @param port - порт БД
     * @return true если подключение успешное, иначе false
     * @note содержит логику подключения в серверу БД
     */
    bool connect(
            const std::string& server,
            const std::string& database,
            const std::string& user,
            const std::string& password,
            const unsigned int port = 0
    );
    /*!
     * @public Метод осуществляющий переподключение к БД MySQL
     * @return None
     * @note содержит логику переподключения в серверу БД
     */
    void reconnnect();
    /*!
     * @public Метод осуществляющий закрытие подключения к БД MySQL
     * @return None
     * @note содержит логику закрытия подключения в серверу БД
     */
    void close();
    //! @public объект запроса к БД
    MYSQL_RES* query(const char* stmt);
    //! @public вектор результатов выполнения запроса
    QueryResult get_record(const char* stmt);
    //! @public список (вектор) векторов результатов выполнения запроса
    QueryResultList get_record_list(const char* stmt);
private:
    //! @private основной объект подключения к БД
    MYSQL* _connection;
    //! @private порт БД
    unsigned int _port;
    //! @private ip-адрес сервера БД
    std::string _server;
    //! @private имя БД
    std::string _database;
    //! @private логин пользователя БД
    std::string _user;
    //! @private пароль пользователя БД
    std::string _password;
};
#endif /* DATABASE_H */
