// ============================================================================
#ifndef GSM_H
#define GSM_H
// ============================================================================
#include "Arduino.h"
#include "./includes/device/speaker/speaker.h"
#include <ArduinoSTL.h>
#include <avr/pgmspace.h>
#include <SoftwareSerial.h>
// ============================================================================
#define DEFAULT_RX 8
#define DEFAULT_TX 3
#define DEFUALT_POWER_PIN 2
#define DEFAULT_SERIAL_PORT 9600
#define OK 1
#define NOT_OK 2
#define TIMEOUT 3
// ============================================================================
void viewFreeMemory();
// ============================================================================
#define FF(str) String(str).c_str()
typedef std::string buffer;
// ============================================================================
class SoftwareGSM
{
public:
	SoftwareSerial *_gsm_serial;
	SoftwareGSM(
		const short& rx = DEFAULT_RX,
		const short& tx = DEFAULT_TX,
		const long& serial_port = DEFAULT_SERIAL_PORT
	);
	void send(buffer& command);
	void send_answer(buffer& answer);
	// void send_sms(const char* phone_number, const char* text);
	// void call_number(const char* phone_number);
	void cfg();
	void connect_to_server(const char* ip, const char* port);
	void disconnect_server();

	void execute(buffer* serial_buf);
private:
	SoftwareSpeaker* _speaker;
	buffer* _serial_buf;
	bool _is_server_connect;

	void A6_read(String& retry);
	byte A6_wait_for(
		const char* response1,
		const char* response2,
		unsigned int& timeOut
	);
	byte A6_command(
		const char* command,
		const char* response1,
		const char* response2,
		unsigned int timeOut,
		int repetitions
	);

	bool wait_answer(const char* command);
	void remove(buffer& str, char symbol);
	void replace(
	    buffer& subject,
	    const buffer& search,
	    const buffer& replace
	);
	void prepare_buf();
};
// ============================================================================
#endif /* GSM_H */
// ============================================================================
