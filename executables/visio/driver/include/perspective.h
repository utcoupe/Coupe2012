#ifndef PERSPECTIVE_H
#define PERSPECTIVE_H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>

using namespace std;

void getvertices( vector<cv::Point2f> corners, cv::Point2f src[], cv::Point2f dst[], const int board_w, const int board_h );
void ChessboardFinder(cv::Mat& image, cv::Mat& gray, cv::Mat& warpMatrix, bool& warpok, bool& found, bool &lookForChessBoard, cv::Size board_sz);

#endif // PERSPECTIVE_H
