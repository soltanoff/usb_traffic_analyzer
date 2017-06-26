/* =====================================================================================================================
 * File: interaction.h
 * Description: Базовый интерфейс описывающий сетевое взаимодействие
 * Created: 01.02.2017
 * Author: soltanoff
 * ================================================================================================================== */
#ifndef INTERACTION_H
#define INTERACTION_H
/* ================================================================================================================== */
/*!
 * @interface IBaseInteraction
 * Абстрактный класс описывающий основные поля и методы для реализации сетевого взаимодействия объекта
 */
class IBaseInteraction{
public:
    virtual ~IBaseInteraction() = default;
    /*!
     * @public Основной циклический метод класса.
     * @return 0 если работа завершилась без аварий, иначе -1
     * @note основной метод сетевого взаимодействия
     */
    virtual int exec() = 0;
    /*!
     * @public Метод закрывающий сокет объекта.
     * @return None
     * @note закрытие сокета осуществляется с параметром SHUT_WR
     */
    virtual void close() = 0;
protected:
    /*!
     * @protected Метод обработки ответа
     * @param bytesRecv - количество полученных байт (out)
     * @param answer - массив данных, содержащий ответ (out)
     * @return true - данные получены успешно, инче false
     * @note осуществляет обработку полученного ответа по сети
     */
    virtual bool get_answer(int& bytesRecv, char* answer) = 0;
    /*!
     * @protected Метод отправки данных
     * @param msg - массив отправляемых данных
     * @return true - данные отправленны успешно, иначе false
     * @note осуществляет отправку обработанных данных объекта в пункт назначения по протоколу TCP/IP
     */
    virtual bool send_message(const char* msg) = 0;
};
/* ================================================================================================================== */
#endif /* INTERACTION_H */