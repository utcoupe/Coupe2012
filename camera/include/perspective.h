#ifndef PERSPECTIVE_H
#define PERSPECTIVE_H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>

using namespace std;

void getvertices( vector<cv::Point2f> corners, cv::Point2f src[], cv::Point2f dst[], const int board_w, const int board_h );

#endif // PERSPECTIVE_H
