#include "servoControl.h"
#include "pinceControl.h"
#include <Servo.h> 


Servo myservo;  // create servo object to control a servo 
Servo myservo2;



void initServo()
{
	myservo.attach(12);  // attaches the servo on pin 9 to the servo object 
	myservo2.attach(13);  // attaches the servo on pin 9 to the servo object
}


int setServoState( char index, char etat)
{
	char droite,gauche;
	if(AVANT==index)
	{
		droite=IDAVD;
		gauche=IDAVG;
	}
	else if(ARRIERE==index)
	{
		droite=IDARD;
		gauche=IDARG;
	}
	else return 0;

	switch (etat)
	{
		case PINCE_O:
			myservo.write(SERVO_OUVERT_A);
			myservo2.write(SERVO_OUVERT_B);
			break;

		case PINCE_S:
			myservo.write(SERVO_SERRE_A);
			myservo2.write(SERVO_SERRE_B);
			break;

		case PINCE_F:
			myservo.write(SERVO_FERME_A);
			myservo2.write(SERVO_FERME_B);
			break;

		default:
			break;
	}

	return 1;
}


