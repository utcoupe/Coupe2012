#ifndef PARAMETRES_H
#define PARAMETRES_H

#include "detection.h"

//Camera choice: 0 for built-in webcam, 1 for usb webcam
const unsigned int CAMERA_N	= 1;

//chess board Width and Height
const int BOARD_W = 7;
const int BOARD_H = 5;

//Board size in MM and PX
const unsigned int BOARD_WIDTH_MM	= 210;
const unsigned int BOARD_HEIGHT_MM	= 140;
const unsigned int BOARD_WIDTH_PX	= 105;
const unsigned int BOARD_HEIGHT_PX	= 70;

//HSV color filter tolerance
const int HSV_TOLERANCE1 = 50;
const int HSV_TOLERANCE2 = 40;
const int HSV_TOLERANCE3 = 40;

//Bary point filter tolerance en mm for CDs
const unsigned int TOLERANCE_MIN_CD = 40;
const unsigned int TOLERANCE_MAX_CD = 170;

//Bary point filter tolerance en mm for lingots
const unsigned int TOLERANCE_MIN_LINGOT = 50;
const unsigned int TOLERANCE_MAX_LINGOT = 120;

//LINGOT and CD HSV INFO

const int CD_H = 58;
const int CD_S = 8;
const int CD_V = 240;

const int LINGOT_H = 27;
const int LINGOT_S = 240;
const int LINGOT_V = 165;


const int CD_MIN_H = CD_H - HSV_TOLERANCE1;
const int CD_MIN_S = CD_S - HSV_TOLERANCE2;
const int CD_MIN_V = CD_V - HSV_TOLERANCE3;

const int CD_MAX_H = CD_H + HSV_TOLERANCE1;
const int CD_MAX_S = CD_S + HSV_TOLERANCE2;
const int CD_MAX_V = CD_V + HSV_TOLERANCE3;

const int LINGOT_MIN_H = LINGOT_H - HSV_TOLERANCE1;
const int LINGOT_MIN_S = LINGOT_S - HSV_TOLERANCE2;
const int LINGOT_MIN_V = LINGOT_V - HSV_TOLERANCE3;

const int LINGOT_MAX_H = LINGOT_H + HSV_TOLERANCE1;
const int LINGOT_MAX_S = LINGOT_S + HSV_TOLERANCE2;
const int LINGOT_MAX_V = LINGOT_V + HSV_TOLERANCE3;

#endif // PARAMETRES_H
