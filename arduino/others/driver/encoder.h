/*
 * encoder.h
 *
 *  Created on: 13 janv. 2011
 *      Author: HoHen
 */

#ifndef ENCODER_H_
#define ENCODER_H_

#define PIN_LEFT_A 18 //bleu //arriere
#define PIN_LEFT_B 19 //blanc //arriere
#define PIN_RIGHT_A 20 //bleu //avant
#define PIN_RIGHT_B 21 //blanc //avant
#define INTERRUPT_LEFT_A 5
#define INTERRUPT_LEFT_B 4
#define INTERRUPT_RIGHT_A 2
#define INTERRUPT_RIGHT_B 3

#include "pinceControl.h"
#include "wiring.h"


void initEncoders();
void testAR();
void testAV();
void encoderSafe();

/*The most common type of incremental encoder uses two output channels (A and B) to sense position.
 *  the two output channels of the quadrature encoder indicate both position and direction of rotation.
 *  If A leads B, for example, the disk is rotating in a clockwise direction.
 *  If B leads A, then the disk is rotating in a counter-clockwise direction.
 *  */

/*void valueChangeOnEncoderLeftPinA();
void valueChangeOnEncoderLeftPinB();
void valueChangeOnEncoderRightPinA();
void valueChangeOnEncoderRightPinB();*/
void valueChangeOnMSRecalAVBas();
void valueChangeOnMSRecalAVHaut();
void valueChangeOnMSRecalARBas();
void valueChangeOnMSRecalARHaut();

extern long value_left_enc;//AR
extern long value_right_enc;//AV
extern long goal_position_AV;
extern long goal_position_AR;


#endif /* ENCODER_H_ */
