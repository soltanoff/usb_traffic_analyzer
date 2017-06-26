// ============================================================================
#ifndef CONTROLLER_H
#define CONTROLLER_H
// ============================================================================
#include "./includes/traffic/traffic.h"
// ============================================================================
#define MSG_FLAG "+CIEV: \"MESSAGE\""
#define RECEIVE_FLAG "+CIPRCV:"
#define IP_MSG_FLAG "!ip: "
#define RR_MSG_FLAG "!rr"
#define GET_MSG_FLAG "!get"
#define SEND_MSG_FLAG "!send: "
#define NPOSE std::string::npos
// ============================================================================
class ModulesControl {
public:
    static void serial_analyze(
        SoftwareGSM* gsm_module,
	    KeyboardParser* keyboard_prs,
	    buffer* serial_buf
    );
    static void execute(
        SoftwareGSM* gsm_module,
        KeyboardParser* keyboard_prs
    );
};
// ============================================================================
#endif /* CONTROLLER_H */
// ============================================================================
