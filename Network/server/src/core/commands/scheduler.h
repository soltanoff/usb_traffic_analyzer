/* =====================================================================================================================
 * File: scheduler.h
 * Description: Планировщик выполнения команд сервера
 * Created: 17.02.2017
 * Author: soltanoff
 * ================================================================================================================== */
#ifndef SCHEDULER_H
#define SCHEDULER_H
/* ================================================================================================================== */
#include "../../config/config.h"
#include <regex>
#include <functional>
#include <vector>
#include <map>
#include <list>
/* ================================================================================================================== */
//! @typedef тип команды
enum class COMMANDS : std::uint32_t {
    unknown=0,
    help,
    test,
    device,
    command,
    clear_db,
    get_all,
    get_buf,
    get_read,
    get_unread,
    rr,
    set_read_all,
    store,
};
/* ================================================================================================================== */
/*!
 * @class Scheduler
 * Класс осуществляющий планирование выполнения команд сервера
 */
class Scheduler {
public:
    //! @typedef тип функции пост-обработки
    typedef std::function<void(std::vector<std::uint8_t>, SOCKET)> ResultCallback;
    //! @typedef тип функции обработки команд
    typedef std::function<void(std::vector<std::uint8_t>, ResultCallback, SOCKET)> Func;
    /*!
     * @public @static Основной метод обработки команд
     * @param cmd_code - код типа команды
     * @param args - аргументы команды
     * @param s - сокет объекта назначения
     * @return true - команда обработана успешно, иначе false
     * @note осуществляет поиск обработчика команды по коду и осуществляет дальнейшее ее выполнение
     */
    static bool execute_command(const std::uint32_t& cmd_code, const std::string& args, SOCKET& s);
    /*!
     * @public @static Метод планирования обработки команд
     * @param data - данные полученные от клиента
     * @param args - аргументы команды
     * @param is_gsm - флаг GSM-устройства
     * @return true - команда обработана успешно, иначе false
     * @note осуществляет анализ полученных от клиента данных, а также их дальнейшая обработка
     */
    static int schedule(const std::vector<uint8_t> &data, SOCKET s, const bool& is_gsm);
    /*!
     * @private Метод связывающий код команды, функцию обработки команд и результурующей функции
     * @param cmd - код команды
     * @param callback - функция обработки команды
     * @param result_cb - результирующая функция (функция осуществляющая пост-обработку (отправку данных, например))
     * @note осуществляет связку функции обработки команд с конкретным объектом-обработчиком
     */
    static void bind(COMMANDS cmd, Func&& callback, ResultCallback&& result_cb);
private:
    //! @private @static словарь callback'ов результирующей функции и функции-обработчика
    static std::map< std::uint32_t, std::list< std::pair<Func, ResultCallback> > > _callbacks_map;
};

#endif /* SCHEDULER_H */
