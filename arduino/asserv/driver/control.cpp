/*
 * control.cpp
 *
 *  Created on: 13 janv. 2011
 *	  Author: HoHen
 */


/* Quelques trucs sur le PID
 *
 * kp : le terme proportionnel qui permet d'augmenter la vitesse de montee (atteint la consigne le plus rapidement possible).
 * ki : le terme integral qui reduit l'erreur statique.
 * kd : le terme derive qui reduit le depassement (l'overshoot).
 * __________________________________________________________________________________________________
 * Coefficient		|Temps de montee	|Temps de stabilisation	|Depassement	|Erreur statique	|
 *	kp				|Diminue			|Augmente				|Augmente		|Diminue			|
 *	ki				|Diminue			|Augmente				|Augmente		|Annule				|
 *	kd				|-					|Diminue				|Diminue		|-					|
 * _________________|___________________|_______________________|_______________|___________________|
 */

#include "WProgram.h"
#include "control.h"
#include "encoder.h"
#include "robotstate.h"
#include "PID_Beta6.h"
#include "fifo.h"
#include "message.h"
#include "wiring.h"


CurrentGoal current_goal;

void initController(){
	current_goal.isReached = true;
	current_goal.isCanceled = false;
	current_goal.isMessageSent = false;
	current_goal.isPaused = false;
}


double pwm,consigne,currentSpeed;
PID pid4SpeedControl(&currentSpeed,&pwm,&consigne,KP_SPEED,KI_SPEED,KD_SPEED);

/* Calcule les pwm a appliquer pour un asservissement en vitesse en trapeze
 * <> value_pwm_left : la pwm a appliquer sur la roue gauche [-255,255]
 * <> value_pwm_right : la pwm a appliquer sur la roue droite [-255,255]
 * */
void speedControl(int* value_pwm_left, int* value_pwm_right){
	/* si le robot est en train de tourner, et qu'on lui donne une consigne de vitesse, il ne va pas partir droit
	 * solution = decomposer l'asservissement en vitesse en 2 :
	 * -> stopper le robot (les 2 vitesses = 0)
	 * -> lancer l'asservissement en vitesse
	 */

	static int start_time;

	static bool initDone = false;

	if(!initDone){
		start_time = 0;
		pwm = 0;
		currentSpeed = 0;
		consigne = 0;
		pid4SpeedControl.Reset();
		pid4SpeedControl.SetInputLimits(-20000,20000);
		pid4SpeedControl.SetOutputLimits(-255,255);
		pid4SpeedControl.SetSampleTime(DUREE_CYCLE);
		pid4SpeedControl.SetMode(AUTO);
		initDone = true;
	}

	/* Gestion de l'arret d'urgence */
	if(current_goal.isCanceled){
		initDone = false;
		current_goal.isReached = true;
		current_goal.isCanceled = false;
		/* et juste pour etre sur */
		(*value_pwm_right) = 0;
		(*value_pwm_left) = 0;
		return;
	}

	/* Gestion de la pause */
	if(current_goal.isPaused){
		(*value_pwm_right) = 0;
		(*value_pwm_left) = 0;
		return;
	}

	if(current_goal.phase == PHASE_1){ //phase d'acceleration
		consigne = current_goal.speed;
		currentSpeed = robot_state.speed;
		if(abs(consigne-currentSpeed) < 1000){ /*si l'erreur est inferieur a 1, on concidere la consigne atteinte*/
			current_goal.phase = PHASE_2;
			start_time = millis();
		}
	}
	else if(current_goal.phase == PHASE_2){ //phase de regime permanent
		consigne = current_goal.speed;
		currentSpeed = robot_state.speed;
		if(millis()-start_time > current_goal.period){ /* fin de regime permanent */
			current_goal.phase = PHASE_3;
		}
	}
	else if(current_goal.phase == PHASE_3){ //phase de decceleration
		consigne = 0;
		currentSpeed = robot_state.speed;
		if(abs(robot_state.speed)<1000){
			current_goal.phase = PHASE_4;
		}
	}

	pid4SpeedControl.Compute();

	if(current_goal.phase == PHASE_4){
		(*value_pwm_right) = 0;
		(*value_pwm_left) = 0;
		current_goal.isReached = true;
		initDone = false;
	}else{
		(*value_pwm_right) = pwm;
		(*value_pwm_left) = pwm;
	}
}

double currentEcart;
PID pid4AngleControl(&currentEcart,&pwm,&consigne,KP_ANGLE,KI_ANGLE,KD_ANGLE);
/* Calcule les pwm a appliquer pour un asservissement en angle
 * <> value_pwm_left : la pwm a appliquer sur la roue gauche [-255,255]
 * <> value_pwm_right : la pwm a appliquer sur la roue droite [-255,255]
 * */
void angleControl(int* value_pwm_left, int* value_pwm_right){

	static bool initDone = false;

	if(!initDone){
		pwm = 0;
		currentEcart = .0;
		consigne = .0;
		pid4AngleControl.Reset();
		pid4AngleControl.SetInputLimits(-M_PI,M_PI);
		pid4AngleControl.SetOutputLimits(-current_goal.speed,current_goal.speed);
		pid4AngleControl.SetSampleTime(DUREE_CYCLE);
		pid4AngleControl.SetMode(AUTO);
		initDone = true;
	}

	/* Gestion de l'arret d'urgence */
	if(current_goal.isCanceled){
		initDone = false;
		current_goal.isReached = true;
		current_goal.isCanceled = false;
		/* et juste pour etre sur */
		(*value_pwm_right) = 0;
		(*value_pwm_left) = 0;
		return;
	}

	/* Gestion de la pause */
	if(current_goal.isPaused){
		(*value_pwm_right) = 0;
		(*value_pwm_left) = 0;
		return;
	}

	/*l'angle consigne doit etre comprise entre ]-PI, PI]

	En fait pour simplifier, l'entree du PID sera l'ecart entre le l'angle courant et l'angle cible (consigne - angle courant)
	la consigne (SetPoint) du PID sera 0
	la sortie du PID sera le double pwm
	*/
	currentEcart = -moduloPI(current_goal.angle - robot_state.angle);

	/*
	Serial.print("goal: ");
	Serial.print(current_goal.angle);
	Serial.print(" current: ");
	Serial.print(robot_state.angle);
	Serial.print(" ecart: ");
	Serial.println(currentEcart);
	*/

	if(abs(currentEcart) < 3.0f*M_PI/360.0f) /*si l'erreur est inferieur a 3deg, on concidere la consigne atteinte*/
		current_goal.phase = PHASE_2;
	else
		current_goal.phase = PHASE_1;

	pid4AngleControl.Compute();

	if(current_goal.phase == PHASE_2){
		(*value_pwm_right) = 0;
		(*value_pwm_left) = 0;
	}
	else{
		(*value_pwm_right) = pwm;
		(*value_pwm_left) = -pwm;
	}

	if(current_goal.phase == PHASE_2){
		if(current_goal.id != -1 && !current_goal.isMessageSent){
			//le message d'arrivee n'a pas encore ete envoye a l'intelligence
			//envoi du message
			sendMessage(current_goal.id,2);
			current_goal.isMessageSent = true;
		}
		if(!fifoIsEmpty()){ //on passe a la tache suivante
			/*condition d'arret = si on a atteint le but et qu'un nouveau but attends dans la fifo*/
			current_goal.isReached = true;
			initDone = false;
		}
	}
}


/* Asservissement en position
 * Se base sur un representation alpha-delta (polaire en quelquesorte) de l'ecart avec la position desiree
 * Alpha represente l'ecart de l'angle (rho en polaire)
 * Delta represente l'ecart de distance (r en polaire)
 * L'idee est donc de separer la pwm resultante en 2 composantes :
 * 	-la vitesse de rotation pour reduire l'ecart alpha
 * 	-la vitesse lineaire pour reduire l'ecart delta
 */
double output4Delta, output4Alpha;
double currentDelta, currentAlpha;
double consigneDelta, consigneAlpha;
PID pid4DeltaControl(&currentDelta,&output4Delta,&consigneDelta,KP_DELTA,KI_DELTA,KD_DELTA);
PID pid4AlphaControl(&currentAlpha,&output4Alpha,&consigneAlpha,KP_ALPHA,KI_ALPHA,KD_ALPHA);

/* Calcule les pwm a appliquer pour un asservissement en position
 * <> value_pwm_left : la pwm a appliquer sur la roue gauche [-255,255]
 * <> value_pwm_right : la pwm a appliquer sur la roue droite [-255,255]
 * */
void positionControl(int* value_pwm_left, int* value_pwm_right){

	static bool initDone = false;

	if(!initDone){
		output4Delta = 0;
		output4Alpha = 0;
		currentDelta = .0;
		currentAlpha = .0;
		consigneDelta = .0;
		consigneAlpha = .0;
		pid4DeltaControl.Reset();
		pid4DeltaControl.SetInputLimits(-TABLE_DISTANCE_MAX_MM/ENC_TICKS_TO_MM,TABLE_DISTANCE_MAX_MM/ENC_TICKS_TO_MM);
		pid4DeltaControl.SetSampleTime(DUREE_CYCLE);
		pid4DeltaControl.SetOutputLimits(-current_goal.speed,current_goal.speed); /*composante liee a la vitesse lineaire*/
		pid4DeltaControl.SetMode(AUTO);
		pid4AlphaControl.Reset();
		pid4AlphaControl.SetSampleTime(DUREE_CYCLE);
		pid4AlphaControl.SetInputLimits(-M_PI,M_PI);
		pid4AlphaControl.SetOutputLimits(-255,255); /*composante lie a la vitesse de rotation*/
		pid4AlphaControl.SetMode(AUTO);
		initDone = true;
	}

	/* Gestion de l'arret d'urgence */
	if(current_goal.isCanceled){
		initDone = false;
		current_goal.isReached = true;
		current_goal.isCanceled = false;
		/* et juste pour etre sur */
		(*value_pwm_right) = 0;
		(*value_pwm_left) = 0;
		return;
	}

	/* Gestion de la pause */
	if(current_goal.isPaused){
		(*value_pwm_right) = 0;
		(*value_pwm_left) = 0;
		return;
	}


	/*calcul de l'angle alpha a combler avant d'etre aligne avec le point cible
	 * borne = [-PI PI] */
	double angularCoeff = atan2(current_goal.y-robot_state.y,current_goal.x-robot_state.x); /*arctan(y/x) -> [-PI,PI]*/
	currentAlpha = moduloPI(angularCoeff - robot_state.angle); /* il faut un modulo ici, c'est sur !


	/* En fait, le sens est donne par l'ecart entre le coeff angulaire et l'angle courant du robot.
	 * Si cet angle est inferieur a PI/2 en valeur absolue, le robot avance en marche avant (il avance quoi)
	 * Si cet angle est superieur a PI/2 en valeur absolue, le robot recule en marche arriere (= il recule)
	 */
	int sens = 1;
	if(abs(currentAlpha) > M_PI/2){/* c'est a dire qu'on a meilleur temps de partir en marche arriere */
		sens = -1;
		currentAlpha = moduloPI(M_PI + angularCoeff - robot_state.angle);
	}
	
	currentAlpha = -currentAlpha;


 	double dx = current_goal.x-robot_state.x;
	double dy = current_goal.y-robot_state.y;
	currentDelta = -sens * sqrt(dx*dx+dy*dy); // - parce que l'ecart doit etre negatif pour partir en avant

	/*
	Serial.print("coeff:");
	Serial.print(angularCoeff);
	Serial.print("  angle:");
	Serial.print(robot_state.angle);
	Serial.print("  alpha:");
	Serial.print(currentAlpha);
	Serial.print("  delta:");
	Serial.print(currentDelta);
	Serial.print("  x:");
	Serial.print(current_goal.x);
	Serial.print("  y:");
	Serial.println(current_goal.y);
	*/
	
	
	
	/* on limite la vitesse lineaire quand on s'approche du but */
	if (abs(currentDelta)<250){
		pid4DeltaControl.SetOutputLimits(-min(50,current_goal.speed),min(50,current_goal.speed)); // composante liee a la vitesse lineaire
		pid4AlphaControl.SetOutputLimits(-150,150); // composante liee a la vitesse de rotation
	}
	else if (abs(currentDelta)<500){
		pid4DeltaControl.SetOutputLimits(-min(60,current_goal.speed),min(60,current_goal.speed)); // composante liee a la vitesse lineaire
		pid4AlphaControl.SetOutputLimits(-150,150); // composante liee a la vitesse de rotation
	}
	else if (abs(currentDelta)<750){
		pid4DeltaControl.SetOutputLimits(-min(80,current_goal.speed),min(80,current_goal.speed)); // composante liee a la vitesse lineaire
		pid4AlphaControl.SetOutputLimits(-150,150); // composante liee a la vitesse de rotation
	}
	else if (abs(currentDelta)<1000){
		pid4DeltaControl.SetOutputLimits(-min(100,current_goal.speed),min(100,current_goal.speed)); // composante liee a la vitesse lineaire
		pid4AlphaControl.SetOutputLimits(-150,150); // composante liee a la vitesse de rotation
	}
	else if (abs(currentDelta)<1250){
		pid4DeltaControl.SetOutputLimits(-min(150,current_goal.speed),min(150,current_goal.speed)); // composante liee a la vitesse lineaire
		pid4AlphaControl.SetOutputLimits(-150,150); // composante liee a la vitesse de rotation
	}
	else if (abs(currentDelta)<1500){
		pid4DeltaControl.SetOutputLimits(-min(200,current_goal.speed),min(200,current_goal.speed)); // composante liee a la vitesse lineaire
		pid4AlphaControl.SetOutputLimits(-150,150); // composante liee a la vitesse de rotation
	}

	if(abs(currentDelta) < 5*ENC_MM_TO_TICKS) /*si l'ecart n'est plus que de 6 mm, on considere la consigne comme atteinte*/
		current_goal.phase = PHASE_2;
	else
		current_goal.phase = PHASE_1;

	pid4AlphaControl.Compute();
	pid4DeltaControl.Compute();

	if(current_goal.phase == PHASE_2){
		(*value_pwm_right) = 0;
		(*value_pwm_left) = 0;
	}
	else{
		double pwm4Delta = 0.0;

		//FIXME probleme de debordemment
		//alpha 200 delta 255 / delta+alpha = 455 / delta-alpha = 55 / => alpha 200 delta 55
		//alpha 200 delta -255 / delta+alpha = -55 / delta-alpha = -455 / => alpha 200 delta -55
		//alpha -200 delta 255 / delta+alpha = 55 / delta-alpha = 455 / => alpha -200 delta 55
		//alpha -200 delta -255 / delta+alpha = -455 / delta-alpha = -55 / => alpha -200 delta -55
		/*
		 Correction by thomas
		 if(output4Delta+output4Alpha>255 || output4Delta-output4Alpha>255)
			pwm4Delta = 255-output4Alpha;
		else if(output4Delta+output4Alpha<-255 || output4Delta-output4Alpha<-255)
			pwm4Delta = -255+output4Alpha;
		else
			pwm4Delta = output4Delta;*/

		(*value_pwm_right) = output4Delta+output4Alpha;
		(*value_pwm_left) = output4Delta-output4Alpha;
		
		// Correction by thomas
		if ((*value_pwm_right) > 255)
			(*value_pwm_right) = 255;
		else if ((*value_pwm_right) < -255)
			(*value_pwm_right) = -255;
		
		if ((*value_pwm_left) > 255)
			(*value_pwm_left) = 255;
		else if ((*value_pwm_left) < -255)
			(*value_pwm_left) = -255;
	}

	if(current_goal.phase == PHASE_2){
		if(current_goal.id != -1 && !current_goal.isMessageSent){
			//le message d'arrivee n'a pas encore ete envoye a l'intelligence
			//envoi du message
			sendMessage(current_goal.id,2);
			current_goal.isMessageSent = true;
		}
		/*condition d'arret = si on a atteint le but et qu'un nouveau but attends dans la fifo*/
		if(!fifoIsEmpty()){ //on passe a la tache suivante
			current_goal.isReached = true;
			initDone = false;
		}
	}

}

/*
 * Permet l'attente non bloquante sur une periode donnee (suite a un pushGoalDelay)
 */
void delayControl(int* value_pwm_left, int* value_pwm_right){
	static bool initDone = false;
	static unsigned long start = 0;
		
	if(!initDone){
		start = millis();
		initDone = true;
	}

	/* Gestion de l'arret d'urgence */
	if(current_goal.isCanceled){
		initDone = false;
		current_goal.isReached = true;
		current_goal.isCanceled = false;
		/* et juste pour etre sur */
		(*value_pwm_right) = 0;
		(*value_pwm_left) = 0;
		return;
	}

	/* Gestion de la pause */
	if(current_goal.isPaused){
		(*value_pwm_right) = 0;
		(*value_pwm_left) = 0;
		return;
	}

	(*value_pwm_right) = 0;
	(*value_pwm_left) = 0;

	if(millis()-start > current_goal.period){
		current_goal.isReached = true;
		initDone = false;
	}
}

/*
 * Permet l'application d'une pwm fixe pendant un temps donne
 */
void pwmControl(int* value_pwm_left, int* value_pwm_right){
	static bool initDone = false;
	static unsigned long start = 0;
		
	if(!initDone){
		start = millis();
		initDone = true;
	}

	/* Gestion de l'arret d'urgence */
	if(current_goal.isCanceled){
		initDone = false;
		current_goal.isReached = true;
		current_goal.isCanceled = false;
		/* et juste pour etre sur */
		(*value_pwm_right) = 0;
		(*value_pwm_left) = 0;
		return;
	}

	/* Gestion de la pause */
	if(current_goal.isPaused){
		(*value_pwm_right) = 0;
		(*value_pwm_left) = 0;
		return;
	}

	(*value_pwm_right) = current_goal.speed;
	(*value_pwm_left) = current_goal.speed;

	if(millis()-start > current_goal.period){
		current_goal.isReached = true;
		initDone = false;
		sendMessage(current_goal.id, 2);
		(*value_pwm_right) = 0;
		(*value_pwm_left) = 0;
	}
}


/* Implementation du modele d'evolution du robot a partir de l'odometrie
 * A appeler a intervalle regulier (a voir pour la mettre sur une interruption timer)
 * */
void computeRobotState(){
	static long prev_value_left_enc = 0;
	static long prev_value_right_enc = 0;

	static unsigned long prevTime = 0;

	/*calcul du deplacement depuis la derniere fois en ticks */
	long dl = value_left_enc - prev_value_left_enc;
	long dr = value_right_enc - prev_value_right_enc;

	/*preparation de la prochaine iteration*/
	prev_value_left_enc = value_left_enc;
	prev_value_right_enc = value_right_enc;

	/*calcul de la vitesse en mm/s */
	unsigned long duree = micros() - prevTime;
	double speed_left = ((double)dl/(double)duree)*1000000.0;
	double speed_right = ((double)dr/(double)duree)*1000000.0;
	double speed = (speed_left+speed_right)/2.0; /*estimation : simple moyenne*/
	prevTime = micros();

	/* calcul du changement d'orientation en rad */
	double delta_angle = (double)(dr-dl)/(double)ENC_CENTER_DIST_TICKS;

	/* calcul du deplacement */
	double delta_dist = (double)(dr+dl)/2.0;

	/* mise a jour de la position en ticks
	 * on utilise des cos et des sin et c'est pas tres opti.
	 * A voir si l'approximation par un dev limite d'ordre 2 est plus efficace
	 */
	double dx = delta_dist*cos(robot_state.angle+delta_angle); //FIXME y a peut etre un delta_angle/2 ici, a verifier
	double dy = delta_dist*sin(robot_state.angle+delta_angle);

	/*mise a jour de l'etat du robot  */
	robot_state.speed = speed;
	robot_state.angle =  moduloPI(robot_state.angle + delta_angle);
	robot_state.x += dx;
	robot_state.y += dy;
}



/* Fonction de calcul du modulo PI ]-PI,PI]
 * moduloPi( 3*PI/2 )= - PI/2
 * moduloPi( PI ) = PI
 * moduloPi( 0 ) = 0
 *
 * x-y*((int)(x/y)) => c'est le modulo pour des doubles (x%y)
 * trunc et int c'est pareil au final, j'ai choisi trunc juste pour que ce soit plus complique !
 * */
double moduloPI(double Nb){
	double result;
	if (Nb > M_PI)
		result = Nb - (2*M_PI)*trunc((Nb + M_PI) / (2*M_PI));
	else  if (Nb <= -M_PI)
		result = Nb - (2*M_PI)*trunc((Nb - M_PI) / (2*M_PI));
	else result = Nb;
	return(result);
}
