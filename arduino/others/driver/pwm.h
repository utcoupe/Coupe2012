/*
 * pwm.h
 *
 *  Created on: 13 janv. 2011
 *      Author: HoHen
 */

#ifndef PWM_H_
#define PWM_H_

#define PIN_EN_LEFT 3
#define PIN_DIR_LEFT 12
#define PIN_EN_RIGHT 11
#define PIN_DIR_RIGHT 13

void initPWM();
void setAVPWM(int);
void setARPWM(int);

#endif /* PWM_H_ */
