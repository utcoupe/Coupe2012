#ifndef CONFIG_H
#define CONFIG_H
#include "parametres.h"

typedef struct
{
     //Camera choice: 0 for built-in webcam, 1 for usb webcam
     int CAMERA_N;

     //chess board Width and Height
     int BOARD_W;
     int BOARD_H;

     //Board size in MM and PX
     int BOARD_WIDTH_MM;
     int BOARD_HEIGHT_MM;
     int BOARD_WIDTH_PX;
     int BOARD_HEIGHT_PX;

     float RATIO_X;
     float RATIO_Y;

     float RATIOX_MM2PX;
     float RATIOY_MM2PX;


     int WIDTH_WINDOW;
     int HEIGHT_WINDOW;
     int DIS_X_CAM_CR;
     int DIS_Y_CAM_CR;

     //HSV color filter tolerance
     int HSV_TOLERANCE1;
     int HSV_TOLERANCE2;
     int HSV_TOLERANCE3;

     //Bary point filter tolerance en mm for CDs
     int TOLERANCE_MIN_CD;
     int TOLERANCE_MAX_CD;

     //Bary point filter tolerance en mm for lingots
     int TOLERANCE_MIN_LINGOT;
     int TOLERANCE_MAX_LINGOT;

     int TRANS_X;
     int TRANS_Y;
} Config;

extern Config config;


#endif // CONFIG_H
