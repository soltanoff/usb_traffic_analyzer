/* ========================================================================= */
#ifndef KEYBOARD_H
#define KEYBOARD_H
/* ========================================================================= */
#include <Keyboard.h>
#include <hidboot.h>
#include <usbhub.h>
#include <ArduinoSTL.h>
/* ========================================================================= */
class KeyboardParser : public KeyboardReportParser
{
public:
    std::string buffer;
    void send_command(const char* command);
protected:
    void OnControlKeysChanged(uint8_t before, uint8_t after);
    void OnKeyDown(uint8_t mod, uint8_t key);
    void OnKeyUp(uint8_t mod, uint8_t key);
    void OnKeyPressed(uint8_t key);
private:
    void PrintKey(uint8_t mod, uint8_t key);
};
/* ========================================================================= */
#endif /* KEYBOARD_H */
/* ========================================================================= */
