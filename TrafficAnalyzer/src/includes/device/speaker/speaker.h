// ============================================================================
#ifndef SPEAKER_H
#define SPEAKER_H
// ============================================================================
#include "Arduino.h"
// ============================================================================
class SoftwareSpeaker
{
public:
	SoftwareSpeaker(short pin = 0);
	void module_initional();
	// void sms_sending();
	void serial_sending();
	void serial_answer();
private:
	short _pin;
	void speaker_voice(int time = 500, int iteration = 1, int frequency = 0);
};
// ============================================================================
#endif /* SPEAKER_H */
// ============================================================================
