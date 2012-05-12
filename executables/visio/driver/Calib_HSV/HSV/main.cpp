#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>
#include "include/detection.h"
#include <iostream>
#include "include/tools.h"
#include "include/parametres.h"
#include "include/perspective.h"
#include <unistd.h>
#include <sys/param.h>

using namespace std;

void validerCD()
{
     int h, s, v, h2, s2, v2;
     cv::FileStorage fCD("../../CDhsv.yml", cv::FileStorage::WRITE);
     h = cv::getTrackbarPos("h_cd", "Calib_cd");
          s = cv::getTrackbarPos("s_cd", "Calib_cd");
          v = cv::getTrackbarPos("v_cd", "Calib_cd");
          h2 = cv::getTrackbarPos("h2_cd", "Calib_cd");
          s2 = cv::getTrackbarPos("s2_cd", "Calib_cd");
          v2 = cv::getTrackbarPos("v2_cd", "Calib_cd");


               fCD << "h_cd" << h;
               fCD << "s_cd" << s;
               fCD << "v_cd" << v;
               fCD << "h2_cd" << h2;
               fCD << "s2_cd" << s2;
               fCD << "v2_cd" << v2;
               fCD.release();
          cout<<"HSV of CD saved"<<endl;
}

void validerL()
{
          cv::FileStorage fL("../../Lhsv.yml", cv::FileStorage::WRITE);
          int h, s, v, h2, s2, v2;
          h = cv::getTrackbarPos("h_L", "Calib_Lingot");
          s = cv::getTrackbarPos("s_L", "Calib_Lingot");
          v = cv::getTrackbarPos("v_L", "Calib_Lingot");
          h2 = cv::getTrackbarPos("h2_L", "Calib_Lingot");
          s2 = cv::getTrackbarPos("s2_L", "Calib_Lingot");
          v2 = cv::getTrackbarPos("v2_L", "Calib_Lingot");


               fL << "h_L" << h;
               fL << "s_L" << s;
               fL << "v_L" << v;
               fL << "h2_L" << h2;
               fL << "s2_L" << s2;
               fL << "v2_L" << v2;
               fL.release();
          cout<<"HSV of Lingot saved"<<endl;
}

void validerN()
{
     int h, s, v, h2, s2, v2;
          cv::FileStorage fN("../../Nhsv.yml", cv::FileStorage::WRITE);
          h = cv::getTrackbarPos("h_N", "Calib_CDnoir");
          s = cv::getTrackbarPos("s_N", "Calib_CDnoir");
          v = cv::getTrackbarPos("v_N", "Calib_CDnoir");
          h2 = cv::getTrackbarPos("h2_N", "Calib_CDnoir");
          s2 = cv::getTrackbarPos("s2_N", "Calib_CDnoir");
          v2 = cv::getTrackbarPos("v2_N", "Calib_CDnoir");

               fN << "h_N" << h;
               fN << "s_N" << s;
               fN << "v_N" << v;
               fN << "h2_N" << h2;
               fN << "s2_N" << s2;
               fN << "v2_N" << v2;
               fN.release();
          cout<<"HSV of CD noir saved"<<endl;
}

void valider(int indice)
{
     switch (indice) {
     case 0: validerCD();
     break;
     case 1: validerL();
     break;
     case 2: validerN();
     break;
     }
}


void Binary_Filtering(cv::Mat &warped, int indice)
{
     int h, s, v, h2, s2, v2;
     cv::Mat warped_hsv;
     cv::cvtColor(warped, warped_hsv, CV_BGR2HSV);
     cv::Mat Binary_cd, Binary_L, Binary_N;
     switch (indice) {

     case 0:

          h = cv::getTrackbarPos("h_cd", "Calib_cd");
          s = cv::getTrackbarPos("s_cd", "Calib_cd");
          v = cv::getTrackbarPos("v_cd", "Calib_cd");
          h2 = cv::getTrackbarPos("h2_cd", "Calib_cd");
          s2 = cv::getTrackbarPos("s2_cd", "Calib_cd");
          v2 = cv::getTrackbarPos("v2_cd", "Calib_cd");
          cv::inRange(warped_hsv, cv::Scalar(h, s, v), cv::Scalar(h2, s2, v2), Binary_cd);
          cv::imshow( "Binary_cd", Binary_cd );
     break;

     case 1:
          h = cv::getTrackbarPos("h_L", "Calib_Lingot");
          s = cv::getTrackbarPos("s_L", "Calib_Lingot");
          v = cv::getTrackbarPos("v_L", "Calib_Lingot");
          h2 = cv::getTrackbarPos("h2_L", "Calib_Lingot");
          s2 = cv::getTrackbarPos("s2_L", "Calib_Lingot");
          v2 = cv::getTrackbarPos("v2_L", "Calib_Lingot");
          cv::inRange(warped_hsv, cv::Scalar(h, s, v), cv::Scalar(h2, s2, v2), Binary_L);
          cv::imshow( "Binary_L", Binary_L );
     break;

     case 2:
          h = cv::getTrackbarPos("h_N", "Calib_CDnoir");
          s = cv::getTrackbarPos("s_N", "Calib_CDnoir");
          v = cv::getTrackbarPos("v_N", "Calib_CDnoir");
          h2 = cv::getTrackbarPos("h2_N", "Calib_CDnoir");
          s2 = cv::getTrackbarPos("s2_N", "Calib_CDnoir");
          v2 = cv::getTrackbarPos("v2_N", "Calib_CDnoir");
          cv::inRange(warped_hsv, cv::Scalar(h, s, v), cv::Scalar(h2, s2, v2), Binary_N);
          cv::imshow( "Binary_N", Binary_N );
     break;
     }
}

void Calib(int &h, int &s, int &v, int &h2, int &s2, int &v2, cv::Mat &warped, int indice)
{

     switch (indice){
     case 0:
          cv::namedWindow( "Calib_cd" , CV_WINDOW_AUTOSIZE);
          cv::createTrackbar( "h_cd", "Calib_cd", &h, 255 );
          cv::createTrackbar( "s_cd", "Calib_cd", &s, 255 );
          cv::createTrackbar( "v_cd", "Calib_cd", &v, 255 );
          cv::createTrackbar( "h2_cd", "Calib_cd", &h2, 255 );
          cv::createTrackbar( "s2_cd", "Calib_cd", &s2, 255 );
          cv::createTrackbar( "v2_cd", "Calib_cd", &v2, 255 );
          Binary_Filtering(warped, indice);
          cv::namedWindow( "Binary_cd" , CV_WINDOW_AUTOSIZE);
     break;

     case 1:
          cv::namedWindow( "Calib_Lingot" , CV_WINDOW_AUTOSIZE);
          cv::createTrackbar( "h_L", "Calib_Lingot", &h, 255 );
          cv::createTrackbar( "s_L", "Calib_Lingot", &s, 255 );
          cv::createTrackbar( "v_L", "Calib_Lingot", &v, 255 );
          cv::createTrackbar( "h2_L", "Calib_Lingot", &h2, 255 );
          cv::createTrackbar( "s2_L", "Calib_Lingot", &s2, 255 );
          cv::createTrackbar( "v2_L", "Calib_Lingot", &v2, 255 );
          Binary_Filtering(warped, indice);
          cv::namedWindow( "Binary_L" , CV_WINDOW_AUTOSIZE);
     break;

     case 2:
          cv::namedWindow( "Calib_CDnoir" , CV_WINDOW_AUTOSIZE);
          cv::createTrackbar( "h_N", "Calib_CDnoir", &h, 255 );
          cv::createTrackbar( "s_N", "Calib_CDnoir", &s, 255 );
          cv::createTrackbar( "v_N", "Calib_CDnoir", &v, 255 );
          cv::createTrackbar( "h2_N", "Calib_CDnoir", &h2, 255 );
          cv::createTrackbar( "s2_N", "Calib_CDnoir", &s2, 255 );
          cv::createTrackbar( "v2_N", "Calib_CDnoir", &v2, 255 );
          Binary_Filtering(warped, indice);
          cv::namedWindow( "Binary_N" , CV_WINDOW_AUTOSIZE);
     break;
     }
}

int main(int argc, char** argv)
{
	int board_w = BOARD_W;
	int board_h = BOARD_H;
     int h_cd, s_cd, v_cd, h2_cd, s2_cd, v2_cd, h_L, s_L, v_L, h2_L, s2_L, v2_L, h_N, s_N, v_N, h2_N, s2_N, v2_N;
     cv::Vec3b hsv_selected;
     cv::Scalar hsv_pixel;
     bool recharger_Matrix_Perspective = false, havent_recharge_yet = true;
     vector<cv::Point> Positions_Display, Positions_Display_CD, Positions_Display_LINGOT;
     cv::Mat binary;
     cv::Mat warped;
     ParamonMouse paramonmouse;
     bool lookForChessBoard = false;
     bool Calibrer_CD = false;
     bool Calibrer_L = false;
     bool Calibrer_N = false;
     int indice;

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
	cv::namedWindow( "Camera" , CV_WINDOW_AUTOSIZE);
//	cv::namedWindow( "Warped" , CV_WINDOW_AUTOSIZE);
	bool found = false;
	bool warpok = false;

     if(fexists("../../CDhsv.yml") and fexists("../../Lhsv.yml") and fexists("../../Nhsv.yml"))
               {
                    cv::FileStorage fCDr("../../CDhsv.yml", cv::FileStorage::READ);
                    fCDr["h_cd"] >> h_cd;
                    fCDr["s_cd"] >> s_cd;
                    fCDr["v_cd"] >> v_cd;
                    fCDr["h2_cd"] >> h2_cd;
                    fCDr["s2_cd"] >> s2_cd;
                    fCDr["v2_cd"] >> v2_cd;
                    fCDr.release();
                    cv::FileStorage fLr("../../Lhsv.yml", cv::FileStorage::READ);
                    fLr["h_L"] >> h_L;
                    fLr["s_L"] >> s_L;
                    fLr["v_L"] >> v_L;
                    fLr["h2_L"] >> h2_L;
                    fLr["s2_L"] >> s2_L;
                    fLr["v2_L"] >> v2_L;
                    fLr.release();
                    cv::FileStorage fNr("../../Nhsv.yml", cv::FileStorage::READ);
                    fNr["h_N"] >> h_N;
                    fNr["s_N"] >> s_N;
                    fNr["v_N"] >> v_N;
                    fNr["h2_N"] >> h2_N;
                    fNr["s2_N"] >> s2_N;
                    fNr["v2_N"] >> v2_N;
                    fNr.release();
               }
               else {
               cerr << "Failed to open HSVbande.yml! Initialize all to 0! " << endl;
               h_cd = s_cd = v_cd = h2_cd = s2_cd = v2_cd = h_L = s_L = v_L = h2_L
                    = s2_L = v2_L = h_N = s_N = v_N = h2_N = s2_N = v2_N = 0;}

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
            ChessboardFinder(image, gray, warpMatrix, warpok, found, lookForChessBoard, recharger_Matrix_Perspective, board_sz);
		}

		// Show raw_image
		if(havent_recharge_yet) 		cv::imshow( "Camera", image );

		if(recharger_Matrix_Perspective)
		{
		     cv::FileStorage fs2("../../warpMatrix.yml", cv::FileStorage::READ);
		     fs2["warpMatrix"] >> warpMatrix;
		     warpPerspective(image, warped, warpMatrix, image.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT);
		     cv::imshow( "Camera", warped);
		     //recharger_Matrix_Perspective = false;
		}

     if(Calibrer_CD)     {
     Calib(h_cd, s_cd, v_cd, h2_cd, s2_cd, v2_cd, warped, indice); //, h_L, s_L, v_L, h2_L, s2_L, v2_L
     }

     if(Calibrer_L)     {
     Calib(h_L, s_L, v_L, h2_L, s2_L, v2_L, warped, indice);
     }

     if(Calibrer_N)     {
     Calib(h_N, s_N, v_N, h2_N, s2_N, v2_N, warped, indice);
     }

     if(not warped.empty())
     {
        MousePick(warped, binary, Positions_Display_CD, paramonmouse, 2, Positions_Display_LINGOT);
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

			case 'r':
                    recharger_Matrix_Perspective = true;
                    cout<<"matrix recharged"<<endl;
                    havent_recharge_yet = false;
			break;

               case 'v':
                    valider(indice);
                break;

               case 'c':
                    Calibrer_CD = !Calibrer_CD;
                    indice = 0;
                    cout << "Calibrer_CD: " << Calibrer_CD << endl;
                break;

               case 'l':
                    Calibrer_L = !Calibrer_L;
                    indice = 1;
                    cout << "Calibrer_L: " << Calibrer_L << endl;
                break;

                case 'n':
                    Calibrer_N = !Calibrer_N;
                    indice = 2;
                    cout << "Calibrer_N: " << Calibrer_N << endl;
                break;


			case 27:
				return 0;

			default:
				if (c != -1)
					cout << c << endl;
		}

	}

	return 0;
}
