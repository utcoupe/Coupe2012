#ifndef _MISCFUNC_H // if not defined
#define _MISCFUNC_H // define MyHeader

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>
#include "parametres.h"

using namespace std;

cv::Point2f px2mm(cv::Point2f & p);
cv::Point2f px2mm(cv::Point & p);
cv::Point2f mm2px(cv::Point & p);
void px2Cam(cv::Point & p);
void px2mm(vector<cv::Point> & p);
bool fexists(const char *filename);
void id();
void ping();
//void CalibHSV();
void valider();
#endif

