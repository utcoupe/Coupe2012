#ifndef DETECTION_H
#define DETECTION_H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>
#include "parametres.h"
#include "detection.h"

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
void findObjects(cv::Mat src, vector<cv::Point>& VecOfPosition, int indice_objet);
void MousePick(cv::Mat& warped, cv::Mat& binary, const vector<cv::Point>& Positions_Display,
               ParamonMouse& paramonmouse, int index);
void MousePick(cv::Mat& warped, cv::Mat& binary, const vector<cv::Point>& Positions_Display_CD,
               ParamonMouse& paramonmouse, int index, const vector<cv::Point>& Positions_Display_LINGOT);
bool EliminatedContour(vector<cv::Point> contour, cv::Point bary, int index);
#endif // DETECTION_H
