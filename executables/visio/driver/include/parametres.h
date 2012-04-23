#ifndef PARAMETRES_H
#define PARAMETRES_H

#include "detection.h"

//Camera choice: 0 for built-in webcam, 1 for usb webcam
const unsigned int CAMERA_N	= 0;

//chess board Width and Height
const int BOARD_W = 8;
const int BOARD_H = 6;

//Board size in MM and PX
const unsigned int BOARD_WIDTH_MM	= 210;
const unsigned int BOARD_HEIGHT_MM	= 140;
const unsigned int BOARD_WIDTH_PX	= 105;
const unsigned int BOARD_HEIGHT_PX	= 70;

//HSV color filter tolerance
const int HSV_TOLERANCE1 = 15;
const int HSV_TOLERANCE2 = 10;
const int HSV_TOLERANCE3 = 40;

//Bary point filter tolerance en mm for CDs
const unsigned int TOLERANCE_MIN_CD = 40;
const unsigned int TOLERANCE_MAX_CD = 170;

//Bary point filter tolerance en mm for lingots
const unsigned int TOLERANCE_MIN_LINGOT = 40;
const unsigned int TOLERANCE_MAX_LINGOT = 170;

//LINGOT and CD HSV INFO

const int CD_H = 58;
const int CD_S = 8;
const int CD_V = 240;

const int LINGOT_H = 11;
const int LINGOT_S = 216;
const int LINGOT_V = 235;

#endif // PARAMETRES_H