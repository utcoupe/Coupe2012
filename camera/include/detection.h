#ifndef DETECTION_H
#define DETECTION_H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>

using namespace std;

typedef struct
{
	cv::Scalar hsv1, hsv2;
	int h,s,v,h2,s2,v2;
	cv::Mat * image;
	cv::Vec3b hsv_selected;
} ParamonMouse;

cv::Point barycentre (vector<cv::Point> & contour);
void DeterminateHSV (ParamonMouse *parametre);
void onMouse(int event, int x, int y, int flags, void * param);


#endif // DETECTION_H
