#ifndef _MISCFUNC_H // if not defined
#define _MISCFUNC_H // define MyHeader

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>
#include "parametres.h"

using namespace std;

cv::Point2f px2mm(cv::Point2f & p);
cv::Point2f px2mm(cv::Point & p);
void px2mm(vector<cv::Point> & p);
#endif