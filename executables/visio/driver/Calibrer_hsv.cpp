
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>
#include <iostream>
#include "../../include/perspective.h"
#include "../../include/parametres.h"

using namespace std;

void valider()
{
     int h_cd, s_cd, v_cd, h2_cd,s2_cd,v2_cd;
     int h_L, s_L, v_L, h2_L, s2_L, v2_L;

     h_cd = cv::getTrackbarPos("h_cd", "Tools");
     s_cd = cv::getTrackbarPos("s_cd", "Tools");
     v_cd = cv::getTrackbarPos("v_cd", "Tools");
     h2_cd = cv::getTrackbarPos("h2_cd", "Tools");
     s2_cd = cv::getTrackbarPos("s2_cd", "Tools");
     v2_cd = cv::getTrackbarPos("v2_cd", "Tools");

     h_L = cv::getTrackbarPos("h_L", "Tools");
     s_L = cv::getTrackbarPos("s_L", "Tools");
     v_L = cv::getTrackbarPos("v_L", "Tools");
     h2_L = cv::getTrackbarPos("h2_L", "Tools");
     s2_L = cv::getTrackbarPos("s2_L", "Tools");
     v2_L = cv::getTrackbarPos("v2_L", "Tools");

     cv::FileStorage fhsv("HSVbande.yml", cv::FileStorage::WRITE);

     fhsv << "h_cd" << h_cd;
     fhsv << "s_cd" << s_cd;
     fhsv << "v_cd" << v_cd;
     fhsv << "h2_cd" << h2_cd;
     fhsv << "s2_cd" << s2_cd;
     fhsv << "v2_cd" <<v2_cd;

     fhsv << "h_L" <<h_L;
     fhsv << "s_L" << s_L;
     fhsv << "v_L" << v_L;
     fhsv << "h2_L" << h2_L;
     fhsv << "s2_L" << s2_L;
     fhsv << "v2_L" << v2_L;

     fhsv.release();
     cout<<"HSV saved"<<endl;
}


void CalibHSV()
{
     int h_cd, s_cd, v_cd, h2_cd,s2_cd,v2_cd;
     int h_L, s_L, v_L, h2_L, s2_L, v2_L;

     if(fexists("HSVbande.yml"))
     {
          cv::FileStorage fhsv("HSVbande.yml", cv::FileStorage::READ);
          fhsv["h_cd"] >> h_cd;
          fhsv["s_cd"] >> s_cd;
          fhsv["v_cd"] >> v_cd;
          fhsv["h2_cd"] >> h2_cd;
          fhsv["s2_cd"] >> s2_cd;
          fhsv["v2_cd"] >> v2_cd;

          fhsv["h_L"] >> h_L;
          fhsv["s_L"] >> s_L;
          fhsv["v_L"] >> v_L;
          fhsv["h2_L"] >> h2_L;
          fhsv["s2_L"] >> s2_L;
          fhsv["v2_L"] >> v2_L;
          fhsv.release();
     }
     else
     {
          h_cd=s_cd=v_cd=h2_cd=s2_cd=v2_cd=
          h_L = s_L = v_L = h2_L = s2_L = v2_L = 0;
     }

	cv::namedWindow( "Tools" , CV_WINDOW_AUTOSIZE);

	cv::createTrackbar( "h_cd", "Tools", &h_cd, 255 );
	cv::createTrackbar( "s_cd", "Tools", &s_cd, 255 );
	cv::createTrackbar( "v_cd", "Tools", &v_cd, 255 );
	cv::createTrackbar( "h2_cd", "Tools", &h2_cd, 255 );
	cv::createTrackbar( "s2_cd", "Tools", &s2_cd, 255 );
	cv::createTrackbar( "v2_cd", "Tools", &v2_cd, 255 );

	cv::createTrackbar( "h_L", "Tools", &h_L, 255 );
	cv::createTrackbar( "s_L", "Tools", &s_L, 255 );
	cv::createTrackbar( "v_L", "Tools", &v_L, 255 );
	cv::createTrackbar( "h2_L", "Tools", &h2_L, 255 );
	cv::createTrackbar( "s2_L", "Tools", &s2_L, 255 );
	cv::createTrackbar( "v2_L", "Tools", &v2_L, 255 );

     cv::inRange(warped_hsv, cv::Scalar(h_cd, s_cd, v_cd), cv::Scalar(h2_cd, s2_cd, v2_cd), binary_cd);
     cv::inRange(warped_hsv, cv::Scalar(h_L, s_L, v_L), cv::Scalar(h2_L, s2_L, v2_L), binary_lingot);

     cv::imshow( "binary_cd", binary_cd );
     cv::imshow( "binary_lingot", binary_lingot );
}

int main(int argc, char** argv)
{
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

		if(recharger_Matrix_Perspective)
		{
		     cv::FileStorage fs2("warpMatrix.yml", cv::FileStorage::READ);
		     fs2["warpMatrix"] >> warpMatrix;
		     warpPerspective(image, warped, warpMatrix, image.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT);
		     cv::imshow( "Warped", warped);
		     //recharger_Matrix_Perspective = false;
		}

     CalibHSV();

        if(not warped.empty())
     {
        pthread_mutex_lock( &lock );
        MousePick(warped, binary, Positions_Display_CD, paramonmouse, 2, Positions_Display_LINGOT);
        pthread_mutex_unlock( &lock );
        }/home/siqi/UTcoupe/Coupe2012/executables/visio/driver/Calib_HSV/Calib_hsv/Calib_hsv.cbp
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

               case 'v':
               valider();
                break;

               case 'r'
                    recharger_Matrix_Perspective = !recharger_Matrix_Perspective;
				cout << "Recharger la matrice" << recharger_Matrix_Perspective << endl;

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



