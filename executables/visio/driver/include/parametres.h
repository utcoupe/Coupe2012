#ifndef PARAMETRES_H
#define PARAMETRES_H

//Camera choice: 0 for built-in webcam, 1 for usb webcam
const unsigned int CAMERA_N	= 1;

//chess board Width and Height
const int BOARD_W = 8;
const int BOARD_H = 6;

//Board size in MM and PX
const unsigned int BOARD_WIDTH_MM	= 210;
const unsigned int BOARD_HEIGHT_MM	= 140;
const unsigned int BOARD_WIDTH_PX	= 105;
const unsigned int BOARD_HEIGHT_PX	= 70;

//HSV color filter tolerance
const int HSV_TOLERANCE1 = 5;
const int HSV_TOLERANCE2 = 10;
const int HSV_TOLERANCE3 = 40;

//Bary point filter tolerance en mm
const unsigned int TOLERANCE_MIN = 40;
const unsigned int TOLERANCE_MAX = 170;
#endif // PARAMETRES_H
