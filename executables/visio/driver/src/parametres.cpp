#include "../include/parametres.h"
#include "../include/tools.h"
#include "../include/config.h"

void getconfig(Config &config, string direct)
{

     if(fexists(direct.c_str()))
     {
     cv::FileStorage fConfig(direct, cv::FileStorage::READ);
     fConfig["CAMERA_N"] >> config.CAMERA_N;
     fConfig["BOARD_W"] >> config.BOARD_W;
     fConfig["BOARD_H"] >> config.BOARD_H;
     fConfig["BOARD_WIDTH_PX"] >> config.BOARD_WIDTH_PX;
     fConfig["BOARD_HEIGHT_PX"] >> config.BOARD_HEIGHT_PX;
     fConfig["WIDTH_WINDOW"] >> config.WIDTH_WINDOW;
     fConfig["HEIGHT_WINDOW"] >> config.HEIGHT_WINDOW;
     fConfig["DIS_X_CAM_CR"] >> config.DIS_X_CAM_CR;
     fConfig["DIS_Y_CAM_CR"] >> config.DIS_Y_CAM_CR;
     fConfig["HSV_TOLERANCE1"] >> config.HSV_TOLERANCE1;
     fConfig["HSV_TOLERANCE2"] >> config.HSV_TOLERANCE2;
     fConfig["HSV_TOLERANCE3"] >> config.HSV_TOLERANCE3;
     fConfig["TOLERANCE_MIN_CD"] >> config.TOLERANCE_MIN_CD;
     fConfig["TOLERANCE_MAX_CD"] >> config.TOLERANCE_MAX_CD;
     fConfig["TOLERANCE_MIN_LINGOT"] >> config.TOLERANCE_MIN_LINGOT;
     fConfig["TOLERANCE_MAX_LINGOT"] >> config.TOLERANCE_MAX_LINGOT;
     fConfig["TRANS_X"] >> config.TRANS_X;
     fConfig["TRANS_Y"] >> config.TRANS_Y;
     config.BOARD_WIDTH_MM	= 29 * config.BOARD_W;
     config.BOARD_HEIGHT_MM	= 29 * config.BOARD_H;

     config.RATIO_X = (float)config.BOARD_WIDTH_MM/config.BOARD_WIDTH_PX;
     config.RATIO_Y = (float)config.BOARD_HEIGHT_MM/config.BOARD_HEIGHT_PX;

     config.RATIOX_MM2PX = (float)config.BOARD_WIDTH_PX/config.BOARD_WIDTH_MM;
     config.RATIOY_MM2PX = (float)config.BOARD_HEIGHT_PX/config.BOARD_HEIGHT_MM;
     }
     else cerr<<"Config not found: "<<direct<<endl;

}
