/**
 * \file 	Visio_Main.cpp
 * \author 	Xavier RODRIGUEZ
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
bool bDisplayPosition = false;
vector<cv::Point> Positions_Display;
cv::Mat binary;
pthread_mutex_t lock;



void findObjects(cv::Mat src, vector<cv::Point>& result) {

    vector<vector<cv::Point> > contours;

    vector<cv::Vec4i> hierarchy;

    cv::findContours( binary, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );

    result.resize(contours.size());

    for (unsigned int i=0; i < contours.size(); i++)
    {
        cv::Point bary;
        vector<cv::Point> contour = contours[i];
        bary = barycentre(contour);
        result[i] = bary;
    }
}


/**
 * récupérer la position des cds et lingots
 * @return ((x1,y1),(x2,y2),(x3,y3),..){SEP}((x1,y1),(x2,y2),(x3,y3),..) cds{SEP}lingots
 */

void DisplayPosition()
{

    pthread_mutex_lock( &lock );
    findObjects(binary, Positions_Display);
    pthread_mutex_unlock( &lock );



    stringstream ss;
    ss << "(";
        for (unsigned int i=0; i < Positions_Display.size(); i++)
		{
            ss << "(" << Positions_Display[i].x << "," << Positions_Display[i].y << ")";
		}
    ss << ")+()";
    cout<<ss.str();

    bDisplayPosition = true;
}


int main(int argc, char** argv)

{
	// Pas de new, il faut récupérer le singleton
	// c'est l'unique instance du manager
    ComManager* cm = ComManager::getComManager();

    lock = cm->getMutex();

	// Premiére commande à mettre le kill
	// c'est la commande qui stop le manager
//	cm->addKill(CMD_KILL);


	cm->addFunction(QV_GET, &DisplayPosition);
//	cm->addFunction(CMD2,&fonction2);


	/*
	Pour partager 1 mutex entre 2 processus
	il est possible de récupérer le mutex du manager   			getMutex
	il est aussi possible de lui appliquer un mutex déja créé 	setMutex

	cm->setMutex(mutex_Dun_Autre_Processus);

	*/

	// Une fois tous les paramétres ok, on lance le manager
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
	cv::Mat warped_hsv;

	cv::namedWindow( "Raw" , CV_WINDOW_AUTOSIZE);
	cv::namedWindow( "Warped" , CV_WINDOW_AUTOSIZE);



//	cvSetMouseCallback("Raw", onMouseP, 0);

	/*cv::namedWindow( "Tools" , CV_WINDOW_AUTOSIZE);
	int h,s,v,h2,s2,v2;
	cv::createTrackbar( "h", "Tools", &h, 255 );
	cv::createTrackbar( "s", "Tools", &s, 255 );
	cv::createTrackbar( "v", "Tools", &v, 255 );
	cv::createTrackbar( "h2", "Tools", &h2, 255 );
	cv::createTrackbar( "s2", "Tools", &s2, 255 );
	cv::createTrackbar( "v2", "Tools", &v2, 255 );*/

	vector<cv::Point2f> corners;
//	int corner_count;
	bool found = false;
   	cv::Mat gray;

	cv::Point2f src_vertices[4];
	cv::Point2f dst_vertices[4];


	cv::Mat warpMatrix;
	bool warpok = false;


	vector<cv::Vec3f> circles;

	while(1)
	{
		// Get current frame
		capture >> image;
		if (image.empty())
		{
			cerr << "image empty" << endl;
			continue;
		}

		//cv::Size s = image.size();
		//cout << s.width << " " << s.height << endl;

		// Convert to gray
		cv::cvtColor(image, gray, CV_RGB2GRAY);

		if (lookForChessBoard)
		{
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
                getvertices( corners, src_vertices, dst_vertices, board_w, board_h );
				warpMatrix = cv::getPerspectiveTransform(src_vertices, dst_vertices);

				warpok = true;
				found = false;
				lookForChessBoard= false;
			}
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
		paramonmouse.image = &warped;

		cvSetMouseCallback( "Warped", onMouse, &paramonmouse);

		cv::cvtColor(warped, warped_hsv, CV_BGR2HSV);
		cv::GaussianBlur(warped_hsv, warped_hsv, cv::Size (9, 9), 2, 2);


        pthread_mutex_lock( &lock );
		cv::inRange(warped_hsv, paramonmouse.hsv1, paramonmouse.hsv2, binary);
		cv::GaussianBlur(binary, binary, cv::Size (9, 9), 2, 2);
        pthread_mutex_unlock( &lock );

        if (bDisplayPosition) {
            for (unsigned int i=0; i < Positions_Display.size(); i++) {
                cv::circle(warped, Positions_Display[i], 3, cv::Scalar(0,255,8), -1, 200, 0);
                cout<<"Position Display x: "<<Positions_Display[i].x<<" y:"<<Positions_Display[i].y;
            }
            bDisplayPosition = false;
        }


		// calcul des barycentres

		cv::imshow( "Warped", warped );

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
