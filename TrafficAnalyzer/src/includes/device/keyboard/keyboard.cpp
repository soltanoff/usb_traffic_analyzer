// ============================================================================
#include "./includes/device/keyboard/keyboard.h"
// ============================================================================
String to_hex(uint8_t val, int lvl) {
    String temp = "";
    int num_nibbles = sizeof (uint8_t) * 2;

    do {
        char v = 48 + (((val >> (num_nibbles - 1) * 4)) & 0x0f);
        if(v > 57) v += 7;
        temp += v;
    } while(--num_nibbles);
    return temp;
}
// ============================================================================
void KeyboardParser::send_command(const char* command) {
    Keyboard.releaseAll();
    Keyboard.print(command);
    Serial.print(F("[KEYBOARD] COMMAND: "));
    Serial.println(command);
}
// ============================================================================
void KeyboardParser::PrintKey(uint8_t m, uint8_t key) {
    MODIFIERKEYS mod;
    *((uint8_t*)&mod) = m;
    Serial.print((mod.bmLeftCtrl == 1) ? "C" : " ");
    Serial.print((mod.bmLeftShift == 1) ? "S" : " ");
    Serial.print((mod.bmLeftAlt == 1) ? "A" : " ");
    Serial.print((mod.bmLeftGUI == 1) ? "G" : " ");

    Serial.print(F(" > "));
    Serial.print(to_hex(key, 0x80));
    Serial.println(F(" < "));

    Serial.print((mod.bmRightCtrl == 1) ? "C" : " ");
    Serial.print((mod.bmRightShift == 1) ? "S" : " ");
    Serial.print((mod.bmRightAlt == 1) ? "A" : " ");
    Serial.println((mod.bmRightGUI == 1) ? "G" : " ");
}
// ============================================================================
void KeyboardParser::OnKeyDown(uint8_t mod, uint8_t key) {
    String spec_key = to_hex(key, 0x80);
    uint8_t c = OemToAscii(0, key);

    // Serial.print(F("DN "));
    // Serial.println(c);
    // this->modifiers = mod;
    // Keyboard.set_modifiers(mod);
    this->PrintKey(mod, key);

    if (c && c != 19)
        OnKeyPressed(c);
    else {
        if (spec_key.equals("52")) Keyboard.press(KEY_UP_ARROW);
        if (spec_key.equals("51")) Keyboard.press(KEY_DOWN_ARROW);
        if (spec_key.equals("50")) Keyboard.press(KEY_LEFT_ARROW);
        if (spec_key.equals("4F")) Keyboard.press(KEY_RIGHT_ARROW);
        if (spec_key.equals("2A")) Keyboard.press(KEY_BACKSPACE);
        if (spec_key.equals("2B")) Keyboard.press(KEY_TAB);
        if (spec_key.equals("28")) Keyboard.press(KEY_RETURN);
        if (spec_key.equals("29")) Keyboard.press(KEY_ESC);
        if (spec_key.equals("4C")) Keyboard.press(KEY_DELETE);
        if (spec_key.equals("4B")) Keyboard.press(KEY_PAGE_UP);
        if (spec_key.equals("4E")) Keyboard.press(KEY_PAGE_DOWN);
        if (spec_key.equals("4A")) Keyboard.press(KEY_HOME);
        if (spec_key.equals("4D")) Keyboard.press(KEY_END);
        if (spec_key.equals("39")) Keyboard.press(KEY_CAPS_LOCK);
        if (spec_key.equals("3A")) Keyboard.press(KEY_F1);
        if (spec_key.equals("3B")) Keyboard.press(KEY_F2);
        if (spec_key.equals("3C")) Keyboard.press(KEY_F3);
        if (spec_key.equals("3D")) Keyboard.press(KEY_F4);
        if (spec_key.equals("3E")) Keyboard.press(KEY_F5);
        if (spec_key.equals("3F")) Keyboard.press(KEY_F6);
        if (spec_key.equals("40")) Keyboard.press(KEY_F7);
        if (spec_key.equals("41")) Keyboard.press(KEY_F8);
        if (spec_key.equals("42")) Keyboard.press(KEY_F9);
        if (spec_key.equals("43")) Keyboard.press(KEY_F10);
        if (spec_key.equals("44")) Keyboard.press(KEY_F11);
        if (spec_key.equals("45")) Keyboard.press(KEY_F12);
    }
}
// ============================================================================
void KeyboardParser::OnControlKeysChanged(uint8_t before, uint8_t after) {
    MODIFIERKEYS beforeMod;
    *((uint8_t*)&beforeMod) = before;

    // Keyboard.set_modifiers(after);
    // HID_SendReport(2, &(Keyboard.send_report()), sizeof(Keyboard.send_report()));

    Serial.print(F("[KEYBOARD] OnControlKeysChanged"));
    MODIFIERKEYS afterMod;
    *((uint8_t*)&afterMod) = after;

    if (beforeMod.bmLeftCtrl != afterMod.bmLeftCtrl) {
        Serial.println("[KEYBOARD] LeftCtrl changed");
    }
    if (beforeMod.bmLeftShift != afterMod.bmLeftShift) {
        Serial.println("[KEYBOARD] LeftShift changed");
    }
    if (beforeMod.bmLeftAlt != afterMod.bmLeftAlt) {
        Serial.println("[KEYBOARD] LeftAlt changed");
    }
    if (beforeMod.bmLeftGUI != afterMod.bmLeftGUI) {
        Serial.println("[KEYBOARD] LeftGUI changed");
    }
    if (beforeMod.bmRightCtrl != afterMod.bmRightCtrl) {
        Serial.println("[KEYBOARD] RightCtrl changed");
    }
    if (beforeMod.bmRightShift != afterMod.bmRightShift) {
        Serial.println("[KEYBOARD] RightShift changed");
    }
    if (beforeMod.bmRightAlt != afterMod.bmRightAlt) {
        Serial.println("[KEYBOARD] RightAlt changed");
    }
    if (beforeMod.bmRightGUI != afterMod.bmRightGUI) {
        Serial.println("[KEYBOARD] RightGUI changed");
    }
    else Serial.println("[KEYBOARD] oops");
}
// ============================================================================
void KeyboardParser::OnKeyUp(uint8_t mod, uint8_t key) {
    Serial.print(F("[KEYBOARD] releaseAll\n"));
    Keyboard.releaseAll();
}
// ============================================================================
void KeyboardParser::OnKeyPressed(uint8_t key) {
    Serial.print(F("[KEYBOARD] ASCII: "));

    Serial.println((char)key);

    this->buffer += (char)key;
    Keyboard.press(key);
}
// ============================================================================
