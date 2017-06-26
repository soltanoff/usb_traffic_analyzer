// ============================================================================
#include "./includes/device/mouse/mouse.h"
// ============================================================================
void MouseParser::OnMouseMove(MOUSEINFO *mi) {
    SerialUSB.print(F("dx = "));
    SerialUSB.print(mi->dX, DEC);
    SerialUSB.print(F(" dy = "));
    SerialUSB.println(mi->dY, DEC);
    Mouse.move(mi->dX, mi->dY, 0);
}
// ============================================================================
void MouseParser::OnLeftButtonUp	(MOUSEINFO *mi) {
    SerialUSB.println(F("[MOUSE_CLICK] Left button released"));
    Mouse.release(MOUSE_LEFT);
}
// ============================================================================
void MouseParser::OnLeftButtonDown	(MOUSEINFO *mi) {
    SerialUSB.println(F("[MOUSE_CLICK] Left button pressed"));
    Mouse.press(MOUSE_LEFT);
}
// ============================================================================
void MouseParser::OnRightButtonUp	(MOUSEINFO *mi) {
    SerialUSB.println(F("[MOUSE_CLICK] Right button released"));
    Mouse.release(MOUSE_RIGHT);
}
// ============================================================================
void MouseParser::OnRightButtonDown	(MOUSEINFO *mi) {
    SerialUSB.println(F("[MOUSE_CLICK] Right button pressed"));
    Mouse.press(MOUSE_RIGHT);
}
// ============================================================================
void MouseParser::OnMiddleButtonUp	(MOUSEINFO *mi) {
    SerialUSB.println(F("[MOUSE_CLICK] Middle button released"));
    Mouse.release(MOUSE_MIDDLE);
}
// ============================================================================
void MouseParser::OnMiddleButtonDown	(MOUSEINFO *mi) {
    SerialUSB.println(F("[MOUSE_CLICK] Middle button pressed"));
    Mouse.press(MOUSE_MIDDLE);
}
// ============================================================================
