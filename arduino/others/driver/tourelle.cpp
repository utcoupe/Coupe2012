#include "tourelle.h"
#include "sensor.h"

Stepper motor(2);
int faceTourelleActuel;
int faceTourelleDemande;
bool tourelleActive;

int setupTourelle() {
	long long startTime = millis();
	while(Serial.available() != 0){
		Serial.read();
		if ((millis() - startTime) > SETUP_TIMEOUT)
		{
			sendMessage(E_TOURELLE_SETUP_FAIL, "Erreur setupTourelle().");
			return E_TOURELLE_SETUP_FAIL;
		}
    }
    
    getDistance(PIN_PING_GAUCHE);
    delay(50);
    getDistance(PIN_PING_GAUCHE);
    faceTourelleActuel=AVANT;
    faceTourelleDemande=AVANT;
    tourelleActive=false;
    return 0;
}

int testPing(int pin){
	int tmp = microsecondsToCentimeters(getDistance(pin));
	if(tmp <= MAXDIST)
		return tmp;
	else return DISTANCE_OUTOFRANGE;
}

void updatePosition(int faceUpdate)
{
	if(faceUpdate!=-1)
	{
		tourelleActive=true;
		faceTourelleDemande=faceUpdate;
	}
	
	else 
	{
		tourelleActive=false;
		if(faceTourelleActuel==ARRIERE)
		{
			motor.dir = TURNRIGHT;
			motor.steps = 500;
			motor.run();
			faceTourelleActuel=AVANT;
		}
	}
}

void loopTourelle() 
{	
	static long long timeLastPing=0;
	static bool pingTurnGauche=true;
	static long long timeLastSend=0;
	static int d_gauche = DISTANCE_OUTOFRANGE;
	static int d_droite = DISTANCE_OUTOFRANGE;
	
	if(tourelleActive)
	{
		if ((millis() - timeLastPing) > TEMPO)
		{
			timeLastPing=millis();
			if(pingTurnGauche)
			{
				d_gauche = testPing(PIN_PING_GAUCHE);	
				pingTurnGauche=false;
			}
			else
			{
				d_droite = testPing(PIN_PING_DROITE);
				pingTurnGauche=true;
			}
		}
	
		if(faceTourelleActuel==AVANT && faceTourelleDemande==ARRIERE)
		{
			motor.dir = TURNLEFT;
			motor.steps = 500;
			motor.run();
			faceTourelleActuel=ARRIERE;
		}
		else if(faceTourelleActuel==ARRIERE && faceTourelleDemande==AVANT)
		{
			motor.dir = TURNRIGHT;
			motor.steps = 500;
			motor.run();
			faceTourelleActuel=AVANT;
		}
		else if(millis() - timeLastSend > 200 and (d_gauche < 40 || d_droite < 40))
		{
			timeLastSend = millis();
			int tab[4];
			
			if(d_gauche < 40 && d_droite < 40)
				tab[0]=MIDLE;
			else if(d_gauche<40)
				tab[0]=GAUCHE;
			else
				tab[0]=DROITE;
			
			tab[1]=min(d_gauche, d_droite);
			tab[2]=d_gauche;
			tab[3]=d_droite;
			
			if(faceTourelleActuel==AVANT)
				sendMessage(W_PING_AV, tab, 4);
			else
				sendMessage(W_PING_AR, tab, 4);
				
		}
	}
 }


