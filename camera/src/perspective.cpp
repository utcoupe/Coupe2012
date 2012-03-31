#include "../include/perspective.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>
#include <iostream>
#include "../include/parametres.h"

void getvertices( vector<cv::Point2f> corners, cv::Point2f src[], cv::Point2f dst[], const int board_w, const int board_h )
{
    src[0] = corners[0];
	src[1] = corners[board_w-1];
	src[2] = corners[(board_w*board_h)-1];
	src[3] = corners[board_w*(board_h-1)];
	dst[0] = src[0];
    dst[1] = cv::Point2f(BOARD_WIDTH_PX ,.0) + dst[0];
	dst[2] = cv::Point2f(BOARD_WIDTH_PX, BOARD_HEIGHT_PX) + dst[0];
	dst[3] = cv::Point2f(.0, BOARD_HEIGHT_PX) + dst[0];
}
