#ifndef PARAMETRES_H
#define PARAMETRES_H
#define ActivationVideo 1
#include "detection.h"

//Camera choice: 0 for built-in webcam, 1 for usb webcam
const unsigned int CAMERA_N	= 1;

//chess board Width and Height
const int BOARD_W = 8;
const int BOARD_H = 6;

//Board size in MM and PX
const unsigned int BOARD_WIDTH_MM	= 29 * BOARD_W;
const unsigned int BOARD_HEIGHT_MM	= 29 * BOARD_H;
const unsigned int BOARD_WIDTH_PX	= 110;
const unsigned int BOARD_HEIGHT_PX	= 75;

const float RATIO_X = (float)BOARD_WIDTH_MM/BOARD_WIDTH_PX;
const float RATIO_Y = (float)BOARD_HEIGHT_MM/BOARD_HEIGHT_PX;

const float RATIOX_MM2PX = (float)BOARD_WIDTH_PX/BOARD_WIDTH_MM;
const float RATIOY_MM2PX = (float)BOARD_HEIGHT_PX/BOARD_HEIGHT_MM;


const int WIDTH_WINDOW = 640;
const int HEIGHT_WINDOW = 480;

//HSV color filter tolerance
const int HSV_TOLERANCE1 = 50;
const int HSV_TOLERANCE2 = 40;
const int HSV_TOLERANCE3 = 40;

//Bary point filter tolerance en mm for CDs
const unsigned int TOLERANCE_MIN_CD = 20;
const unsigned int TOLERANCE_MAX_CD = 170;

//Bary point filter tolerance en mm for lingots
const unsigned int TOLERANCE_MIN_LINGOT = 20;
const unsigned int TOLERANCE_MAX_LINGOT = 90;

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
