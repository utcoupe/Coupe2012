#ifndef SERVO_CONTROL_H_
#define SERVO_CONTROL_H_

#define SERVO_FERME_A	50
#define SERVO_FERME_B	160
#define SERVO_SERRE_A	90
#define SERVO_SERRE_B	120
#define SERVO_OUVERT_A	130
#define SERVO_OUVERT_B	70

#define IDAVG		2	//avant gauche
#define IDAVD		3	//avant droit
#define IDARG		7	//arriere gauche
#define IDARD		1	//arriere droit

#define PINCE_F	 	0
#define PINCE_S	 	1
#define PINCE_O	 	2

int setServoState( char index, char etat);
void initServo();

#endif

