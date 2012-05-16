#include "sensor.h"
#include "include_arduino.h"
#include "message.h"

typedef struct
{
	int lastValue;
	long long lastOscillation;
	long long lastSwitch;
	bool oscillation;
}Switch;

Switch switchJack;
void SwitchInit(Switch & s, const int pin)
{
	pinMode(PIN_JACK, INPUT);
	digitalWrite(pin, HIGH);
	s.lastValue = digitalRead(pin);
	s.lastOscillation = 0;
	s.lastSwitch = 0;
	s.oscillation = false;
}
void checkSwitch(const int pin, const int warning, Switch & s, char* param = (char*)"");


void initSensor(){
	SwitchInit(switchJack, PIN_JACK);
}

void checkSwitch(const int pin, const int warning, Switch & s, char* param)
{
	// Si on a pas detecter une oscillation précédement
	// et que la dernière fois qu'on a switch remonte à "longtemps"
	if (!s.oscillation and millis() - s.lastSwitch > 100)
	{
		int v = digitalRead(pin);
		if (v != s.lastValue)
		{
			s.lastOscillation = millis();
			s.oscillation = true;
		}
	}
	// Si on a detecté une oscillation précédement
	// et qu'on a attendu "longtemps" entre temps (pour éliminer les oscillations de merde)
	else if (s.oscillation and millis() - s.lastOscillation > 100)
	{
		int v = digitalRead(pin);
		if (v != s.lastValue)
		{
			if (param != "")
				sendMessage(warning, param, v);
			else
				sendMessage(warning, v);
			s.lastSwitch = millis();
			s.lastValue = v;
		}
		s.oscillation = false;
	}
}


void sensorTrigger() {
	
	checkSwitch(PIN_JACK, ID_MSG_JACK, switchJack);

}



