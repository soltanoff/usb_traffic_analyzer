// ============================================================================
#ifndef MOUSE_H
#define MOUSE_H
// ============================================================================
#include "Mouse.h"
#include <hidboot.h>
#include <usbhub.h>
// ============================================================================
class MouseParser : public MouseReportParser {
protected:
  	void OnMouseMove(MOUSEINFO *mi);
  	void OnLeftButtonUp(MOUSEINFO *mi);
  	void OnLeftButtonDown(MOUSEINFO *mi);
  	void OnRightButtonUp(MOUSEINFO *mi);
  	void OnRightButtonDown(MOUSEINFO *mi);
  	void OnMiddleButtonUp(MOUSEINFO *mi);
  	void OnMiddleButtonDown(MOUSEINFO *mi);
};
// ============================================================================
// MouseParser Prs;
// HIDBoot<USB_HID_PROTOCOL_MOUSE> HidMouse(&Usb);
// 
// void setup() {
//     SerialUSB.begin( 115200 );
//     SerialUSB.println(F("[MOUSE] Run emulator..."));
//
//     if (Usb.Init() == -1)
//         SerialUSB.println(F("[ERROR] OSC did not start."));
//
//     delay(200);
//    // next_time = millis() + 5000;
//     HidMouse.SetReportParser(0, &Prs);
// }
//
// void loop() {
//     Usb.Task();
// }
// ============================================================================
#endif /* MOUSE_H */
// ============================================================================
