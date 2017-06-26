// ============================================================================
#include "./includes/device/gsm/gsm.h"

extern int __bss_end;
extern void *__brkval;
// Функция, возвращающая количество свободного ОЗУ (RAM)
int memoryFree() {
   int freeValue;
   if((int)__brkval == 0)
      freeValue = ((int)&freeValue) - ((int)&__bss_end);
   else
      freeValue = ((int)&freeValue) - ((int)__brkval);
   return freeValue;
}

void viewFreeMemory() {
	Serial.print(F("Free RAM: "));
	Serial.println(memoryFree());
}
// ============================================================================
SoftwareGSM::SoftwareGSM(
    const short& rx,
    const short& tx,
    const long& serial_port
) {
	this->_speaker = new SoftwareSpeaker();

	this->_is_server_connect = false;
	this->_gsm_serial = new SoftwareSerial(rx, tx);

	// Скорость порта для связи Arduino с GSM модулем
	this->_gsm_serial->begin(serial_port);
    // this->cfg();
    // this->_speaker->module_initional();
}
// ============================================================================
void SoftwareGSM::cfg() {
    // this->_gsm_serial->println(FF(F("AT+CFUN?")));
	this->A6_command(FF(F("AT")), FF(F("OK")), FF(F("yy")), 1100, 12);

	// this->A6_command(FF(F("AT+CMEE=2")), FF(F("OK")), FF(F("yy")), 5000, 2);
	this->A6_command(FF(F("AT+CSCS=\"GSM\"")), FF(F("OK")), FF(F("yy")), 5000, 2);
	this->A6_command(FF(F("AT+CMGF=1")), FF(F("OK")), FF(F("yy")), 5000, 2);
	this->A6_command(FF(F("AT+SNFS=0")), FF(F("OK")), FF(F("yy")), 10000, 2);
    this->A6_command(FF(F("AT+CGATT=1")), FF(F("OK")), FF(F("yy")), 5000, 2);
	// this->A6_command(FF(F("ATE0")), FF(F("OK")), FF(F("yy")), 5000, 2);
    this->_speaker->module_initional();
}
// ============================================================================
/*
void SoftwareGSM::send_sms(const char* phone_number, const char* text) {
	this->_gsm_serial->print("AT+CMGS=\"");
	this->_gsm_serial->print(phone_number);
	this->_gsm_serial->write(0x22);
	this->_gsm_serial->write(0x0D);	// hex код возврата коректки
	this->_gsm_serial->write(0x0A);	// hex код новой строки
	// delay(2000);
	this->_gsm_serial->print(text);
	delay(100);
	this->_gsm_serial->println(char(26));	// ASCII код Сtrl+Z
	// delay(1000);
	// this->_speaker->sms_sending();
}
*/
// ============================================================================
/*
void SoftwareGSM::call_number(const char* phone_number) {
	this->_gsm_serial->print("ATD");
	this->_gsm_serial->println(phone_number);
	delay(10000);
	this->_gsm_serial->println("ATH");
}
*/
// ============================================================================
void SoftwareGSM::remove(buffer& str, char symbol) {
    auto pos = str.find(symbol);
    while (pos != buffer::npos) {
        str.erase(pos, 1);
        pos = str.find(symbol, pos+1);
    }
}
void SoftwareGSM::replace(
    buffer& subject,
    const buffer& search,
    const buffer& replace
) {
    size_t pos = 0;
    while((pos = subject.find(search, pos)) != buffer::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}
void SoftwareGSM::prepare_buf() {
	remove(*_serial_buf, '\r');
    replace(*_serial_buf, FF(F("\n\n")), FF(F(" | ")));
    remove(*_serial_buf, '\n');
}
// ============================================================================
void SoftwareGSM::A6_read(String& retry) {
	retry = "";
	if (this->_gsm_serial->available())
    	retry = this->_gsm_serial->readString();
}
// ============================================================================
byte SoftwareGSM::A6_wait_for(
	const char* response1,
	const char* response2,
	unsigned int& timeOut
) {
  	unsigned long entry = millis();
    String retry;
  	this->A6_read(retry);
	byte retVal = 99;
	do {
		this->A6_read(retry);
	    if (retry.length()) {
			Serial.print((millis() - entry));
		    Serial.print(F(" ms "));
	    	Serial.println(retry);
	    }
	}
    while (
		(retry.indexOf(response1) + retry.indexOf(response2) == -2)
		&& millis() - entry < timeOut
	);
	if ((millis() - entry) >= timeOut) {
		retVal = TIMEOUT;
	}
	else {
		if (retry.indexOf(response1) + retry.indexOf(response2) > -2)
			retVal = OK;
		else
			retVal = NOT_OK;
	}
	return retVal;
}
// ============================================================================
byte SoftwareGSM::A6_command(
	const char* command,
	const char* response1,
	const char* response2,
	unsigned int timeOut,
	int repetitions
) {
	byte returnValue = NOT_OK;
	byte count = 0;
	while (count < repetitions && returnValue != OK) {
		this->_gsm_serial->println(command);
		Serial.print(F("Command: "));
		Serial.println(command);
		if (this->A6_wait_for(response1, response2, timeOut) == OK)
			returnValue = OK;
		else
			returnValue = NOT_OK;
		count++;
	}
	return returnValue;
}
// ============================================================================
void SoftwareGSM::connect_to_server(const char* ip, const char* port) {
    std::string *temp = new std::string;
	Serial.println(F("Connect to server..."));
    this->A6_command(FF(F("AT+CIPCLOSE")), FF(F("OK")), FF(F("yy")), 5000, 1);
	this->A6_command(FF(F("AT+CGATT=1")), FF(F("OK")), FF(F("yy")), 10000, 2);

    *temp = FF(F("AT+CIPSTART=\"TCP\",\""));
    *temp += ip;
	*temp += FF(F("\", "));
	*temp += port;
	this->A6_command(temp->c_str(), FF(F("OK")), FF(F("yy")), 10000, 2);
	this->A6_command(FF(F("AT+CIPSEND=1, \"2\"")), FF(F("OK")), FF(F("yy")), 10000, 1);

	this->_is_server_connect = true;

    Serial.println(F("Connected"));
    this->_speaker->serial_sending();
    delete temp;
	/*
	this->_gsm_serial->println("AT+CGATT=1"); // // Прикрепиться сети если 1,
	// открепиться если 0 (длительность: 8 сек)
	// Задаем контекст поключения PDP (Packet Data Protocol)
	// 1 - индентификатор контекста PDP (max 7)
	// "IP" - тип протокола (PDP type: IP, IPV6, PPP Point to Point Protocol)
	// "cmnet" - имя точки доступа
	this->_gsm_serial->println("AT+CGDCONT=1,\"IP\",\"cmnet\"");
	// Активируем заданный контекст (длительность: 3-4 сек)
	// 1 - состояние (вкл/выкл)
	// 1 - индентификатор контекста PDP
	this->_gsm_serial->println("AT+CGACT=1,1");
	// Начать TCP/UDP подключение
	// (длительность: 2-3 сек)
	this->_gsm_serial->println("AT+CIPSTART=\"TCP\",\"" + ip + "\", " + port);
	this->_gsm_serial->println("AT+CIPSEND=4, \"help\"");
	this->_is_server_connect = true;
	*/
}
// ============================================================================
void SoftwareGSM::disconnect_server() {
	this->A6_command(FF(F("AT+CIPSEND=5, \"close\"")), FF(F("OK")), FF(F("yy")), 10000, 2);
	this->A6_command(FF(F("AT+CIPCLOSE")), FF(F("OK")), FF(F("yy")), 5000, 1);
	this->_is_server_connect = false;
    this->_speaker->serial_sending();
}
// ============================================================================
void SoftwareGSM::send_answer(buffer& answer) {
	std::string *data = new std::string;

	*data = FF(F("AT+CIPSEND="));
	*data += FF(answer.size());
	*data += FF(F(",\""));
	*data += answer.c_str();
	*data += FF(F("\""));

	Serial.print(F("Free RAM: "));
	Serial.println(memoryFree());
    this->_gsm_serial->println(data->c_str());
	// this->A6_command(data.c_str(), FF(F("OK")), FF(F("yy")), 10000, 1);
    delete data;
}
// ============================================================================
void SoftwareGSM::send(buffer& command) {
    this->_gsm_serial->println(command.c_str());
}
// ============================================================================
void SoftwareGSM::execute(buffer *serial_buf) {
    // =========================
    // Clear SRAM
	Serial.flush();
	this->_gsm_serial->flush();
    _serial_buf = serial_buf;
    // =========================
	// GSM MODULE SERIAL
	if (this->_gsm_serial->available()) {
		while (this->_gsm_serial->available()) {
			*_serial_buf += char(this->_gsm_serial->read());
			delay(1);
		}
		this->prepare_buf();
		Serial.println(_serial_buf->c_str());
		viewFreeMemory();
		this->_speaker->serial_answer();
	}
	// =======================================================================
	// SOFTWARE SERIAL
	if (Serial.available()) {
		*_serial_buf = "";
		while (Serial.available()) {
			*_serial_buf += char(Serial.read());
			delay(1);
		}
		this->prepare_buf();
		Serial.println(_serial_buf->c_str());
		this->send(*_serial_buf);
		this->_speaker->serial_sending();
	}
}
// ============================================================================
