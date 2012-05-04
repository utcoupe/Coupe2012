#include "../include/perspective.h"
#include <opencv/cv.h>
#include "opencv2/opencv.hpp"
#include <opencv/highgui.h>
#include <vector>
#include <iostream>
#include "../include/parametres.h"
#include "../include/tools.h"

void getvertices( vector<cv::Point2f> corners, cv::Point2f src[], cv::Point2f dst[], const int board_w, const int board_h )
{
     src[0] = corners[0];
	src[1] = corners[board_w-1];
	src[2] = corners[(board_w*board_h)-1];
	src[3] = corners[board_w*(board_h-1)];


	//dst[0] = src[0];

	cv::Point Trans(-100, 400);
	mm2px(Trans);
	cout<<"Tx"<<Trans.x<<" Ty "<<Trans.y<<endl;
	dst[0].x = WIDTH_WINDOW/2 + Trans.x; dst[0].y = HEIGHT_WINDOW - Trans.y;
     dst[1] = cv::Point2f(BOARD_WIDTH_PX ,.0) + dst[0];
	dst[2] = cv::Point2f(BOARD_WIDTH_PX, BOARD_HEIGHT_PX) + dst[0];
	dst[3] = cv::Point2f(.0, BOARD_HEIGHT_PX) + dst[0];

}

void ChessboardFinder(cv::Mat& image, cv::Mat& gray, cv::Mat& warpMatrix, bool& warpok, bool& found, bool &lookForChessBoard, cv::Size board_sz)
{
            vector<cv::Point2f> corners;
            cv::cvtColor(image, gray, CV_RGB2GRAY);
            cv::Point2f src_vertices[4];
            cv::Point2f dst_vertices[4];
			// Looking for the chessboard
			found = cv::findChessboardCorners( gray, board_sz, corners, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS );

			// If chessboard has been found
			if (found)
			{
				// Get subpixel accuracy on those corners
				cv::cornerSubPix( gray, corners, cv::Size( 11, 11 ),
                    cv::Size( -1, -1 ), cv::TermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1 ));
				// Draw chessboard corners
				cv::drawChessboardCorners( image, board_sz, cv::Mat(corners), found );
				//calculate warpmatrix
                    getvertices( corners, src_vertices, dst_vertices, board_sz.width, board_sz.height);
				warpMatrix = cv::getPerspectiveTransform(src_vertices, dst_vertices);
				cv::FileStorage fs("/home/siqi/UTcoupe/Coupe2012/executables/visio/driver/warpMatrix.yml", cv::FileStorage::WRITE);
                    fs << "warpMatrix" << warpMatrix;
				fs.release();
				warpok = true;
				found = false;
				lookForChessBoard= false;

			}
}
