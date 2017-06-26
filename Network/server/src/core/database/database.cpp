/* =====================================================================================================================
 * File: database.h
 * Description: Подключение к базе данных MySQL сервера
 * Created: 17.02.2017
 * Author: soltanoff
 * ================================================================================================================== */
#include "database.h"


CDBConnection::CDBConnection(
        const std::string& server,
        const std::string& database,
        const std::string& user,
        const std::string& password,
        const unsigned int port
)
{
    connect(server, database, user, password, port);
};

bool CDBConnection::connect(
        const std::string& server,
        const std::string& database,
        const std::string& user,
        const std::string& password,
        const unsigned int port
) {
    _connection = mysql_init(NULL);
    _server = server;
    _database = database;
    _user = user;
    _password = password;
    _port = port;

    if (!mysql_real_connect(
            _connection,
            _server.c_str(),
            _user.c_str(),
            _password.c_str(),
            _database.c_str(),
            _port,
            NULL,
            0
    )) {
        throw std::string("[DATABASE] Connection failed: ") + mysql_error(_connection);
    }
    return true;
}

void CDBConnection::reconnnect() {
    mysql_reload(_connection);
}

void CDBConnection::close() {
    mysql_close(_connection);
}

MYSQL_RES* CDBConnection::query(const char* stmt) {
    // set_db();

    if (mysql_query(_connection, stmt)) {
        throw std::string("[DATABASE] Connection failed: ") + mysql_error(_connection);
    }

    return mysql_use_result(_connection);
}

QueryResult CDBConnection::get_record(const char* stmt) {
    MYSQL_ROW row;
    MYSQL_RES* res = query(stmt);
    QueryResult result;

    if ((row = mysql_fetch_row(res)) != NULL) {
        for (unsigned int i = 0; i < res->field_count; i++) {
            result.push_back(std::string(row[i]));
        }
    }

    mysql_free_result(res);
    return result;
}

QueryResultList CDBConnection::get_record_list(const char* stmt) {
    MYSQL_ROW row;
    MYSQL_RES* res = query(stmt);
    QueryResultList result;

    while ((row = mysql_fetch_row(res)) != NULL) {
        QueryResult result_row;
        for (unsigned int i = 0; i < res->field_count; i++) {
            result_row.push_back(std::string(row[i]));
        }
        result.push_back(result_row);
    }

    mysql_free_result(res);
    return result;
}
