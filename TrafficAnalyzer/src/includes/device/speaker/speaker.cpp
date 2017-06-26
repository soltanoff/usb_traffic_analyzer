// ============================================================================
#include "./includes/device/speaker/speaker.h"
// ============================================================================
void SoftwareSpeaker::speaker_voice(int time, int iteration, int frequency)
{
	for (int i = 5; i <= 5 * iteration; i += 5)
	{
		if (frequency)
			tone(this->_pin, frequency);
		else
			tone(this->_pin, i);

		delay(time);
		noTone(this->_pin);
		delay(time);
	}
}
// ============================================================================
SoftwareSpeaker::SoftwareSpeaker(short pin)
{
	this->_pin = pin;
	pinMode(this->_pin, OUTPUT);
	this->speaker_voice(40, 2, 800);
}
// ============================================================================
void SoftwareSpeaker::module_initional()
{
	this->speaker_voice(40, 3, 500);
}
// ============================================================================
/*
void SoftwareSpeaker::sms_sending()
{
	this->speaker_voice(40, 1, 40);
}
*/
// ============================================================================
void SoftwareSpeaker::serial_sending()
{
	this->speaker_voice(40, 1, 120);
}
// ============================================================================
void SoftwareSpeaker::serial_answer()
{
	this->speaker_voice(80, 1, 500);
}
// ============================================================================
