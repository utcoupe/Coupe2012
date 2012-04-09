/**
 * \file 	Visio_Main.cpp
 * \author 	Liu
 * \date	01/04/2012          // et c'est pas une blague !!!
 *
 * */


#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>
#include <iostream>
#include <sstream>
#include "include/tools.h"
#include "include/detection.h"
#include "include/perspective.h"
#include "include/parametres.h"
#include "comManager.h"
#include "protocole.h"


using namespace std;

cv::Vec3b hsv_selected;
cv::Scalar hsv_pixel;
bool hsv_selected_ready = false;
vector<cv::Point> Positions_Display;
cv::Mat binary;
pthread_mutex_t lock;





/**
 * récupérer la position des cds et lingots
 * @return ((x1,y1),(x2,y2),(x3,y3),..){SEP}((x1,y1),(x2,y2),(x3,y3),..) cds{SEP}lingots
 */


void DisplayPosition()
{
    vector<cv::Point> Positions_Display_Temp;

    pthread_mutex_lock( &lock );
    findObjects(binary, Positions_Display);
    pthread_mutex_unlock( &lock );
    Positions_Display_Temp = Positions_Display;
    stringstream ss;
    ss << "(";
        for (unsigned int i=0; i < Positions_Display_Temp.size(); i++)
		{
		    px2mm(Positions_Display_Temp[i]);
            ss << "(" << Positions_Display_Temp[i].x << "," << Positions_Display_Temp[i].y << ")";
		}
    ss << ")+()";
    cout<<ss.str();
    ss.str("");
}

int main(int argc, char** argv)

{
    ComManager* cm = ComManager::getComManager();

    lock = cm->getMutex();
	cm->addFunction(QV_GET, &DisplayPosition);
	cm->start();

	//code main()

	bool lookForChessBoard = false;
	ParamonMouse paramonmouse;

	int board_w = BOARD_W;
	int board_h = BOARD_H;

	if (argc >= 3)
	{
		board_w = atoi(argv[1]);
		board_h = atoi(argv[2]);
	}

//	int board_n = board_w * board_h;
	cv::Size board_sz = cv::Size( board_w, board_h );
//	const int board_case_px = 20;


	// Try video input (or camera)
	cv::VideoCapture capture(CAMERA_N);
	if (!capture.isOpened())
	{
		cerr << "Failed to open a video device or video file!\n" << endl;
		return 0;
	}
	cv::Mat image;
	cv::Mat warped;
    cv::Mat gray;
	cv::Mat warpMatrix;
	cv::namedWindow( "Raw" , CV_WINDOW_AUTOSIZE);
	cv::namedWindow( "Warped" , CV_WINDOW_AUTOSIZE);
	vector<cv::Vec3f> circles;
	bool found = false;
	bool warpok = false;

	while(1)
	{
		// Get current frame
		capture >> image;
		if (image.empty())
		{
			cerr << "image empty" << endl;
			continue;
		}
		// Convert to gray


        if (lookForChessBoard)
		{
            ChessboardFinder(image, gray, warpMatrix, warpok, found, lookForChessBoard, board_sz);
		}

		// Show raw_image
		cv::imshow( "Raw", image );

		if (warpok)
		{
			// Warp image
			warpPerspective(image, warped, warpMatrix, image.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT);
			// Show warped image
			cv::imshow( "Warped", warped);
		}

		if(not warped.empty())
		{
        pthread_mutex_lock( &lock );
        MousePick(warped, binary, Positions_Display, paramonmouse);
        pthread_mutex_unlock( &lock );
        }
		// Handle pause/unpause and ESC
		int c = cv::waitKey( 15 );
		switch (c)
		{
			case 's':
				lookForChessBoard = !lookForChessBoard;
				cout << "lookForChessBoard" << lookForChessBoard << endl;
			break;

			case 'p':
				c = 0;
				while( c != 'p' && c != 27 ){
					c = cvWaitKey( 250 );
				}
			break;

			case 'c':
                DisplayPosition();
                break;

			case 27:
				return 0;

			default:
				if (c != -1)
					cout << c << endl;
		}

	}

	// Ne pas oublier le wait final
	cm->waitHere();

	return 0;
}
