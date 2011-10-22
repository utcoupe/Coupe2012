#include "sensor.h"


TriggerSharp trigerSharp[NB_SHARP];
int pingAvMessageID;
int pingArMessageID;

typedef struct
{
	int lastValue;
	long long lastOscillation;
	long long lastSwitch;
	bool oscillation;
}Switch;

Switch switchColor, switchJack, switchMSAV, switchMSAR;
void SwitchInit(Switch & s, const int pin)
{
	s.lastValue = digitalRead(pin);
	s.lastOscillation = 0;
	s.lastSwitch = 0;
	s.oscillation = false;
}
void checkSwitch(const int pin, const int warning, Switch & s, char* param = (char*)"");



void initSensor(){
	// micro switch
	pinMode(PIN_MS_AV, INPUT);  
	pinMode(PIN_MS_AR, INPUT);
	// jack et couleur
	pinMode(PIN_COLOR, INPUT);
	pinMode(PIN_JACK, INPUT);
	// let bleu et rouge
	pinMode(LED_BLEU, OUTPUT);
	pinMode(LED_ROUGE, OUTPUT);
	digitalWrite(LED_BLEU,LOW);
	digitalWrite(LED_ROUGE,LOW);
	
		
	pingAvMessageID=-42;
	pingArMessageID=-42;
	
	SwitchInit(switchColor, PIN_COLOR);
	SwitchInit(switchJack, PIN_JACK);
	SwitchInit(switchMSAV, PIN_MS_AV);
	SwitchInit(switchMSAR, PIN_MS_AR);
}

void sensorTrigger()
{
	static long long timePing = 0;

	checkSwitch(PIN_COLOR, W_SWITCH_COLOR, switchColor);
	checkSwitch(PIN_JACK, W_JACK, switchJack);
	
	char buffer[10];
	sprintf(buffer, "%d", AVANT);
	checkSwitch(PIN_MS_AV, W_MS, switchMSAV, buffer);
	sprintf(buffer, "%d", ARRIERE);
	checkSwitch(PIN_MS_AR, W_MS, switchMSAR, buffer);
	
	if (millis() - timePing > 200)
	{
		timePing = millis();
		int d = 0;
		
		if(pingAvMessageID>=0){
			d = microsecondsToCentimeters(getDistance(PIN_PING_AV));
			if(d != 0 and d <= DISTANCE_DETECT){
				sendMessage(W_PING_AV,d);
			}
		}
			
		if(pingArMessageID>=0){
			d = microsecondsToCentimeters(getDistance(PIN_PING_AR));
			if(d != 0 and d <= DISTANCE_DETECT){
				sendMessage(W_PING_AR,d);
			}
		}
	}
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


int setLED( char color){
	if(color == BLEU){
		digitalWrite(LED_BLEU,HIGH);
		digitalWrite(LED_ROUGE,LOW);
		return 1;
	}
	if(color == ROUGE){
		digitalWrite(LED_BLEU,LOW);
		digitalWrite(LED_ROUGE,HIGH);
		return 1;
	}
	
	//erreur donc on allume les 2
	digitalWrite(LED_BLEU,HIGH);
	digitalWrite(LED_ROUGE,HIGH);
	return -42;
}

int getColor()
{
	int c = digitalRead(PIN_COLOR);
	if (c == LOW)
		return BLEU;
	else
		return ROUGE;
}

int getSharp( char pin)
{
  	switch (pin)
  	{
		case 0:
			return analogRead(A0);
		case 1:
			return analogRead(A1);
		case 2:
			return analogRead(A2);
		case 3:
			return analogRead(A3);
		case 4:
			return analogRead(A4);
		case 5:
			return analogRead(A5);
		case 6:
			return analogRead(A6);
		case 7:
			return analogRead(A7);
		case 8:
			return analogRead(A8);
		case 9:
			return analogRead(A9);
		case 10:
			return analogRead(A10);
		case 11:
			return analogRead(A11);
		default:
			return -1;
	}
}

//déclanché si la valeur es inferieur à ref
int setTriggerSharp( int id,  char pin, int ref){
	if(pin > NB_SHARP) return 0; //erreur
	for(int i=0;i<NB_SHARP;i++){
		//remplacement de trigger
		if(trigerSharp[i].pin==pin){
			sendMessage(trigerSharp[i].messageId,0);//annule le precedent trriger
			trigerSharp[i].value=ref;
			trigerSharp[i].messageId=id;
			return 1;
		}
		//nouveau trigger
		if(trigerSharp[i].pin==-1){
			trigerSharp[i].pin=pin;
			trigerSharp[i].value=ref;
			trigerSharp[i].messageId=id;
			return 1;
		}
	}
	return 0;//erreur
}

void removeTriggerSharp(char index){
		// aucun trigger
		if(trigerSharp[0].pin==-1) return;
		//1 trigger
		if(trigerSharp[1].pin==-1){
			trigerSharp[1].pin=-1;//index == 1 ici j'espere
			return;
		}
		//au moin 2 trigger
		for(int j=2;j<NB_SHARP;j++){
			if(trigerSharp[j].pin==-1){
				//remplace le trigger index par le dernier trigger
				trigerSharp[index].pin=trigerSharp[j-1].pin;
				trigerSharp[index].value=trigerSharp[j-1].value;
				trigerSharp[index].messageId=trigerSharp[j-1].messageId;
				trigerSharp[j-1].pin=-1;
				return;
			}
		}
		//tableau de trigger remplis remplace index par le dernier delete le dernier
		trigerSharp[index].pin=trigerSharp[NB_SHARP-1].pin;
		trigerSharp[index].value=trigerSharp[NB_SHARP-1].value;
		trigerSharp[index].messageId=trigerSharp[NB_SHARP-1].messageId;
		trigerSharp[NB_SHARP-1].pin=-1;
}

int getMicroSwitch( char pin){
	switch (pin)
  	{
		case 0:
			if(digitalRead(PIN_MS_AV)==HIGH) return 1; else return 0;
		case 1:
			if(digitalRead(PIN_MS_AR)==HIGH) return 1; else return 0;
		default:
			return 2;
	}
}

int getPion( char face){
	int value,value2;
	//test des sharp du bas vers le haut
	if(face == AVANT){
		//test 1
		value = getSharp(SHARP_AV1);
		if(value>DIST_MAX1 && value<DIST_MIN1){
			//au moin 1 pion
			//test 2 sharp type 2 (value ou value 2)
			value = getSharp(SHARP_AV2G);
			value2 = getSharp(SHARP_AV2D);
			if((value>DIST_MAX2 && value<DIST_MIN2) || (value>DIST_MAX2 && value<DIST_MIN2)){
				//test 3 sharp type 1
				value = getSharp(SHARP_AV3);
				if(value>DIST_MAX1 && value<DIST_MIN1){
					//test 4
					value = getSharp(SHARP_AV4);
					if(value>DIST_MAX1 && value<DIST_MIN1){
						//test 5
						value = getSharp(SHARP_AV5);
						if(value>DIST_MAX1 && value<DIST_MIN1){
									return 5; // tete sur 2 pion
						} else return 4; // tete sur pion
					} else return 3; // tete
				}else return 2; // 2 pion
			}else return 1; // 1 pion seul
		}else return 0; //pas de pion
	}
	
	/*if (millis() - temps > 200)
	{
		temps = millis();
		int d = 0;
		
		if(pingAvMessageID>=0){
			d = microsecondsToCentimeters(getDistance(PIN_PING_AV));
			if(d <= DISTANCE_DETECT){
					sendMessage(-30,d);
			}
		}
			
		if(pingArMessageID>=0){
			d = microsecondsToCentimeters(getDistance(PIN_PING_AR));
			if(d <= DISTANCE_DETECT){
					sendMessage(-31,d);
			}
		}
	}*/
}

