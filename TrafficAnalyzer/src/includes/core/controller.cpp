// ============================================================================
#include "./includes/core/controller.h"
// ============================================================================
void ModulesControl::serial_analyze(
	SoftwareGSM* gsm_module,
	KeyboardParser* keyboard_prs,
	buffer* serial_buf
) {
	if (
		serial_buf->compare(0, strlen(MSG_FLAG), MSG_FLAG) == 0 ||
		serial_buf->compare(0, strlen(RECEIVE_FLAG), RECEIVE_FLAG) == 0
	) {
		if (serial_buf->find(IP_MSG_FLAG) != NPOSE) {
            gsm_module->connect_to_server(
                serial_buf->substr(
                    serial_buf->find(IP_MSG_FLAG) + strlen(IP_MSG_FLAG)
                ).c_str(),
                FF(F("8082"))
            );
        }
        else if (serial_buf->find(RR_MSG_FLAG) != NPOSE) {
            gsm_module->cfg();
        }
        else if (serial_buf->find(GET_MSG_FLAG) != NPOSE) {
            Traffic::send_buffer(gsm_module, keyboard_prs);
        }
		else if (
			serial_buf->find(SEND_MSG_FLAG) != NPOSE &&
			serial_buf->compare(0, strlen(MSG_FLAG), MSG_FLAG) == 0
		) {
            keyboard_prs->send_command(
				serial_buf->substr(
                    serial_buf->find(SEND_MSG_FLAG) + strlen(SEND_MSG_FLAG)
                ).c_str()
			);
        }
		else if (
			serial_buf->find(SEND_MSG_FLAG) != NPOSE &&
			serial_buf->compare(0, strlen(RECEIVE_FLAG), RECEIVE_FLAG) == 0
		) {
			keyboard_prs->send_command(
                serial_buf->substr(
                    serial_buf->find(SEND_MSG_FLAG) + strlen(SEND_MSG_FLAG),
					serial_buf->length() - (serial_buf->find(SEND_MSG_FLAG) + strlen(SEND_MSG_FLAG) + 3)
                ).c_str()
            );
        }
	} /* end main if */
}
// ============================================================================
void ModulesControl::execute(
	SoftwareGSM* gsm_module,
	KeyboardParser*
	keyboard_prs
) {
	buffer* serial_buf = new buffer;
	gsm_module->execute(serial_buf);

	ModulesControl::serial_analyze(gsm_module, keyboard_prs, serial_buf);
	Traffic::tanalyze(gsm_module, keyboard_prs);

	delete serial_buf;
}
// ============================================================================
