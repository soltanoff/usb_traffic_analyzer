/* =====================================================================================================================
 * File: clientinteraction.h
 * Description: Реализация полного взаимодействия с сетевыми клиентми
 * Created: 01.02.2017
 * Author: soltanoff
 * ================================================================================================================== */
#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H
/* ================================================================================================================== */
#include "../config/config.h"
#include "../core/commands/scheduler.h"
#include "../core/commands/functions.h"
#include "interaction.h"
/* ================================================================================================================== */
/*!
 * @class ClientInteraction
 * @inherit IBaseInteraction
 * Абстрактный класс описывающий основные поля и методы для реализации сетевого взаимодействия объекта
 */
class ClientInteraction: public IBaseInteraction {
public:
    ClientInteraction(std::uint32_t client_number, SOCKET client_socket, bool is_gsm);  // , std::mutex& server_mutex);
    ~ClientInteraction() { close(); }
    /*!
     * @public Основной циклический метод класса.
     * @return 0 если работа завершилась без аварий, иначе -1
     * @note основной метод сетевого взаимодействия
     */
    int exec();
    /*!
     * @public Метод закрывающий сокет объекта.
     * @return None
     * @note закрытие сокета осуществляется с параметром SHUT_WR
     */
    void close();
private:
    //! @private порядковый номер клиента
    std::uint32_t _client_number;
    //! @private активный сокет клиента
    SOCKET _client_socket;
    //! @private объект содержащий функции-обработчики команд сервера
    Functions _functions;
    //! флаг GSM-устройства
    bool _is_gsm;
    /*!
     * @private Метод обработки ответа клиента
     * @param bytesRecv - количество полученных байт (out)
     * @param answer - массив данных, содержащий ответ (out)
     * @return true - данные получены успешно, инче false
     * @note осуществляет обработку полученного ответа клиента по сети
     */
    bool get_answer(std::int32_t& bytesRecv, char* answer);
    /*!
     * @private Метод отправки данных клиенту
     * @param msg - массив отправляемых данных
     * @return true - данные отправленны успешно, иначе false
     * @note осуществляет отправку обработанных данных объекта клиенту по протоколу TCP/IP
     */
    bool send_message(const char* msg);
    /*!
     * @private Метод отправки данных объекту по сокету
     * @param msg - массив отправляемых данных
     * @param s -
     * @return true - данные отправленны успешно, иначе false
     * @note осуществляет отправку обработанных данных объекта клиенту по протоколу TCP/IP
     */
    bool send_message(const char* msg, SOCKET s);
    /*!
     * @private Метод связывающий функции обработки команд с объектом-обработчиком
     * @param fptr - функция обработки команды
     * @param optr - объект-обработчик
     * @note осуществляет связку функции обработки команд с конкретным объектом-обработчиком
     */
    template<class TFunc, class TObj>
    auto bind(TFunc&& fptr, TObj& optr) -> decltype(std::bind(fptr, optr, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)) {
        return std::bind(fptr, optr, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    }
};
/* ================================================================================================================== */
#endif /* SERVERTHREAD_H */
