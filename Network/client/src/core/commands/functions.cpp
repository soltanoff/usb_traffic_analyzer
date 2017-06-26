/* =====================================================================================================================
 * File: functions.h
 * Description: Реализации функций обработки команд
 * Created: 14.02.2017
 * Author: soltanoff
 * ================================================================================================================== */
#include "scheduler.h"
#include "functions.h"


// ====================================================================================================================
// SERVICE FUNCTIONS
// function: get database connection
std::unique_ptr<CDBConnection> Functions::get_db() {
    return std::make_unique<CDBConnection>(
            ServerCfg::DB_ADDRESS,
            ServerCfg::DATABASE,
            ServerCfg::DB_USER,
            ServerCfg::DB_PASSWORD
    );
}
// function: get reformat query result
std::string reformatQueryResult(QueryResultList &records) {
    std::string separator;
    std::string result = "\n";
    for (auto it = records.begin(); it != records.end(); it++) {
        for (auto col = (*it).begin(); col != (*it).end(); col++) {
            separator = col == (*it).end() - 1 ? "" : " | ";
            result += (*col) + separator;
        }
        result += "\n";
    }
    return result;
}
// ====================================================================================================================
// COMMAND FUNCTIONS
// cmd: None
void Functions::unknown(DataVector arg, FuncArg result_cb, SOCKET s) {
    std::string str = {" > unknown command"};
    result_cb(DataVector(str.begin(), str.end() + 1), s);
}

// cmd: help
void Functions::help(DataVector arg, FuncArg result_cb, SOCKET s) {
    std::string str = {
            "\nAvailable commands:\n"
                    "> test [{arg}] \t\t - function of the test. Return message.\n"
                    "> device \t\t\t - method of checking the connection of the GSM device.\n"
                    "> command {arg} \t - method of sending commands to GSM device.\n"
                    "> clear_db \t\t\t - set all messages as deleted.\n"
                    "> get_all \t\t\t - get all stored messages.\n"
                    "> get_buf \t\t\t - method of getting data from GSM device buffer.\n"
                    "> get_read \t\t\t - get all read messages.\n"
                    "> get_unread \t\t - get all unread messages.\n"
                    "> rr \t\t\t\t - method of restarting GSM device.\n"
                    "> set_read_all \t\t - set all messages as read.\n"
                    "> store {arg} \t\t - method of storing in DB send {args}.\n"
    };
    result_cb(std::vector<std::uint8_t>(str.begin(), str.end() + 1), s);
}

extern std::vector<SOCKET> GSM_MODULES_SOCKETS;
// cmd: test
void Functions::test(DataVector arg, FuncArg result_cb, SOCKET s) {
    std::string str_arg(arg.begin(), arg.end());

    if (str_arg.empty())
        std::cout << "foo() executed. arg = nullptr :)" << std::endl;
    else
        std::cout << "foo() executed. arg = " << str_arg << std::endl;

    std::string str = {" > this is a test method"};

    for (auto it = GSM_MODULES_SOCKETS.begin(); it != GSM_MODULES_SOCKETS.end(); it++) {
        send(*it, "!send: TEEEST", strlen("!send: TEEEST"), 0);
    }
    result_cb(DataVector(str.begin(), str.end() + 1), s);
}

// cmd: rr
void Functions::rr(DataVector arg, FuncArg result_cb, SOCKET s) {
    std::string str_arg(arg.begin(), arg.end());

    std::string str = " > executed";
    if (GSM_MODULES_SOCKETS.size() > 0) {
        for (auto it = GSM_MODULES_SOCKETS.begin(); it != GSM_MODULES_SOCKETS.end(); it++) {
            send(*it, RR_MSG_FLAG, strlen(RR_MSG_FLAG), 0);
        }
    }
    else
        str = " > Command not available. Device NOT connected to server!";

    result_cb(DataVector(str.begin(), str.end() + 1), s);
}

// cmd: get_buf
void Functions::get_buf(DataVector arg, FuncArg result_cb, SOCKET s) {
    std::string str_arg(arg.begin(), arg.end());

    std::string str = " > executed";
    if (GSM_MODULES_SOCKETS.size() > 0) {
        for (auto it = GSM_MODULES_SOCKETS.begin(); it != GSM_MODULES_SOCKETS.end(); it++) {
            send(*it, GET_MSG_FLAG, strlen(GET_MSG_FLAG), 0);
        }
    }
    else
        str = " > Command not available. Device NOT connected to server!";

    result_cb(DataVector(str.begin(), str.end() + 1), s);
}

// cmd: send_command <args>
void Functions::send_command(DataVector arg, FuncArg result_cb, SOCKET s) {
    std::string str_arg(arg.begin(), arg.end());

    std::string str;
    if (GSM_MODULES_SOCKETS.size() > 0) {
        if (!str_arg.empty()) {
            std::string data = SEND_MSG_FLAG;
            data += str_arg;
            for (auto it = GSM_MODULES_SOCKETS.begin(); it != GSM_MODULES_SOCKETS.end(); it++) {
                send(*it, data.c_str(), data.length(), 0);
            }
            str = " > executed";
        } else
            str = "not enough arguments.";
    }
    else
        str = " > Command not available. Device NOT connected to server!";

    result_cb(DataVector(str.begin(), str.end() + 1), s);
}

// cmd: check_device
void Functions::check_device(DataVector arg, FuncArg result_cb, SOCKET s) {
    std::string str_arg(arg.begin(), arg.end());

    std::string str;

    if (GSM_MODULES_SOCKETS.size() > 0)
        str = " > device connected: Ai Thinker Co.LTDA6 V03.03.20160830011H03";
    else
        str = " > device NOT connected to server!";

    result_cb(DataVector(str.begin(), str.end() + 1), s);
}

// cmd: store <args>
void Functions::store(DataVector arg, FuncArg result_cb, SOCKET s) {
    std::string str_arg(arg.begin(), arg.end());

    std::string str;
    if (!str_arg.empty()) {
        auto db = get_db();
        std::string query = "INSERT INTO Logger (message) VALUES (\'" + str_arg + "\')";
        db->query(query.c_str());
        str = " > stored";
    }
    else
        str = "not enough arguments.";

    result_cb(DataVector(str.begin(), str.end() + 1), s);
}

// cmd: get_all
void Functions::get_all(DataVector arg, FuncArg result_cb, SOCKET s) {
    std::string str_arg(arg.begin(), arg.end());
    std::string result;

    if (arg.empty()) {
        auto db = get_db();
        std::string query = ""
                "SELECT l.id, l.date, l.message "
                "FROM Logger l INNER JOIN State s ON l.id = s.id_logger ";
        QueryResultList records = db->get_record_list(query.c_str());

        result = reformatQueryResult(records);
    }
    else
        result = " > getall execute without arguments";

    result_cb(DataVector(result.begin(), result.end() + 1), s);
}

// cmd: get_read
void Functions::get_read(DataVector arg, FuncArg result_cb, SOCKET s) {
    std::string str_arg(arg.begin(), arg.end());
    std::string result;

    if (arg.empty()) {
        auto db = get_db();
        std::string query = ""
                "SELECT l.id, l.message "
                "FROM Logger l INNER JOIN State s ON l.id = s.id_logger AND s.deleted = 0 "
                "WHERE s.is_read = 1";
        QueryResultList records = db->get_record_list(query.c_str());

        result = reformatQueryResult(records);
    }
    else
        result = " > get_read execute without arguments";

    result_cb(DataVector(result.begin(), result.end() + 1), s);
}

// cmd: get_unread
void Functions::get_unread(DataVector arg, FuncArg result_cb, SOCKET s) {
    std::string str_arg(arg.begin(), arg.end());
    std::string result;

    if (arg.empty()) {
        auto db = get_db();
        std::string query = ""
                "SELECT l.id, l.message "
                "FROM Logger l INNER JOIN State s ON l.id = s.id_logger AND s.deleted = 0 "
                "WHERE s.is_read = 0";

        QueryResultList records = db->get_record_list(query.c_str());

        result = reformatQueryResult(records);
        query = "UPDATE State SET is_read = 1, modify_date = CURRENT_TIMESTAMP";
        db->query(query.c_str());
    }
    else
        result = " > get_unread execute without arguments";

    result_cb(DataVector(result.begin(), result.end() + 1), s);
}

// cmd: set_read_all
void Functions::set_read_all(DataVector arg, FuncArg result_cb, SOCKET s) {
    std::string str_arg(arg.begin(), arg.end());
    std::string result;

    if (arg.empty()) {
        auto db = get_db();
        std::string query = "UPDATE State SET is_read = 1, modify_date = CURRENT_TIMESTAMP";
        db->query(query.c_str());
        result = " > all data is read";
    }
    else
        result = " > clear_db execute without arguments";

    result_cb(DataVector(result.begin(), result.end() + 1), s);
}

// cmd: clear_db
void Functions::clear_db(DataVector arg, FuncArg result_cb, SOCKET s) {
    std::string str_arg(arg.begin(), arg.end());
    std::string result;

    if (arg.empty()) {
        auto db = get_db();
        std::string query = "UPDATE State SET deleted = 1, modify_date = CURRENT_TIMESTAMP";
        db->query(query.c_str());
        result = " > database cleared";
    }
    else
        result = " > clear_db execute without arguments";

    result_cb(DataVector(result.begin(), result.end() + 1), s);
}
