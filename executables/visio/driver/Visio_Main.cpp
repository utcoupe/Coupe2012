

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
#include <unistd.h>
#include <sys/param.h>


using namespace std;

cv::Vec3b hsv_selected;
cv::Scalar hsv_pixel;
bool hsv_selected_ready = false;
bool recharger_Matrix_Perspective = false;
vector<cv::Point> Positions_Display, Positions_Display_CD, Positions_Display_LINGOT, Positions_Display_N;
cv::Mat binary;
pthread_mutex_t lock;
cv::Mat warped;
cv::Mat image;
cv::Mat warpMatrix;
ParamonMouse paramonmouse;
bool lookForChessBoard = false;
string direct_abs, direct_m, direct_CDhsv, direct_Lhsv, direct_Nhsv;

void Display_calcul(int indice, vector<cv::Point> &dst1, vector<cv::Point> &dstCD_LIN_N, vector<cv::Point> &dst_temp)
{
    pthread_mutex_lock( &lock );
    MousePick(warped, binary, dst1, paramonmouse, indice, direct_CDhsv, direct_Lhsv, direct_Nhsv);
/*    stringstream sss;
    sss<<indice;
    cv::namedWindow( sss.str()+"binary" , CV_WINDOW_AUTOSIZE);
    cv::imshow( sss.str()+"binary", binary );
    sss.str("");*/
    findObjects(binary, dst1, indice); //0 for CD, 1 for lingot: elimination tolerance depends on this index, 3 pour CD noir.
    pthread_mutex_unlock( &lock );
    dst_temp = dst1;
    dstCD_LIN_N = dst1;
}


void DisplayPosition()
{
    stringstream ss;
    if(!ActivationVideo) {
         warpPerspective(image, warped, warpMatrix, image.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT);}


    vector<cv::Point> Positions_Display_Temp;

    Display_calcul(0, Positions_Display, Positions_Display_CD, Positions_Display_Temp);

    ss << "(";
        for (unsigned int i=0; i < Positions_Display_Temp.size(); i++)
		{
              px2Cam(Positions_Display_Temp[i]);
		    //Cam2CR(Positions_Display_Temp[i]);
		    px2mm(Positions_Display_Temp[i]);

            ss << "(" << Positions_Display_Temp[i].x << "," << Positions_Display_Temp[i].y << "),";
		}
    ss << ")+(";

    Display_calcul(1, Positions_Display, Positions_Display_LINGOT, Positions_Display_Temp);

        for (unsigned int i=0; i < Positions_Display_Temp.size(); i++)
		{
               px2Cam(Positions_Display_Temp[i]);
              // Cam2CR(Positions_Display_Temp[i]);
		    px2mm(Positions_Display_Temp[i]);
            ss << "(" << Positions_Display_Temp[i].x << "," << Positions_Display_Temp[i].y << "),";
		}
    ss << ")+(";


     Display_calcul(2, Positions_Display, Positions_Display_N, Positions_Display_Temp);

     for (unsigned int i=0; i < Positions_Display_Temp.size(); i++)
     {
          px2Cam(Positions_Display_Temp[i]);
         // Cam2CR(Positions_Display_Temp[i]);
          px2mm(Positions_Display_Temp[i]);
          ss << "(" << Positions_Display_Temp[i].x << "," << Positions_Display_Temp[i].y << "),";
     }

    ss << ")";

    cout<<ss.str();
    ss.str("");
}


void PERSPECTIVE_Calibrer()
{
     lookForChessBoard = true;
     cout<<"0";
}

void PERSPECTIVE_Rechargement()
{
     recharger_Matrix_Perspective = true;
     cout<<"0";
}



int main(int argc, char** argv)

{
    ComManager* cm = ComManager::getComManager();

     lock = cm->getMutex();
	cm->addFunction(QV_GET, &DisplayPosition);
	cm->addFunction(QV_ID, &id);
	cm->addFunction(QV_PING, &ping);
	cm->addFunction(QV_CALIB, &PERSPECTIVE_Calibrer);
	cm->addFunction(QV_RECHARGE, &PERSPECTIVE_Rechargement);


	cm->start();

	//code main()

	//get current directory

     direct_abs = argv[0];
     std::string::size_type end = direct_abs.find_last_of('/');
     direct_abs.erase(end);
     cerr<<"absolute path is: "<<direct_abs<<endl;
     direct_m = direct_abs + "/../../warpMatrix.yml";
     direct_CDhsv = direct_abs + "/../../CDhsv.yml";
     direct_Lhsv = direct_abs + "/../../Lhsv.yml";
     direct_Nhsv = direct_abs + "/../../Nhsv.yml";
     cerr<<"CDHSV path is : "<<direct_CDhsv<<endl;
     cerr<<"LHSV path is : "<<direct_Lhsv<<endl;
     cerr<<"NHSV path is : "<<direct_Nhsv<<endl;
     cerr<<"matrix path is: "<<direct_m<<endl;
	int board_w = BOARD_W;
	int board_h = BOARD_H;

     if (argc >= 3)
	{
		board_w = atoi(argv[1]);
		board_h = atoi(argv[2]);
	}

	cv::Size board_sz = cv::Size( board_w, board_h );

	// Try video input (or camera)
	cv::VideoCapture capture(CAMERA_N);
	if (!capture.isOpened())
	{
		cerr << "Failed to open a video device or video file!\n" << endl;
		return 0;
	}

     cv::Mat gray;

     if(fexists(direct_m.c_str()))
     {
     cv::FileStorage fsm2(direct_m, cv::FileStorage::READ);
     fsm2["warpMatrix"] >> warpMatrix;

	if(ActivationVideo) {
     cv::namedWindow( "Raw" , CV_WINDOW_AUTOSIZE);
	cv::namedWindow( "Warped" , CV_WINDOW_AUTOSIZE); 	}
	vector<cv::Vec3f> circles;
	bool found = false;
	bool warpok = false;
	while(1)
	{
	     //cout<<"ça tourne"<<endl;
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
            ChessboardFinder(image, gray, warpMatrix, warpok, found, lookForChessBoard, board_sz, direct_m);
		}

		// Show raw_image
		if(ActivationVideo) {
		cv::imshow( "Raw", image );}

		if (warpok)
		{
			// Warp image
			warpPerspective(image, warped, warpMatrix, image.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT);
			// Show warped image
			if(ActivationVideo) {
			cv::imshow( "Warped", warped);}
		}

		if(recharger_Matrix_Perspective and (ActivationVideo==1))
		{
		     warpPerspective(image, warped, warpMatrix, image.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT);
		     cv::imshow( "Warped", warped);
		     //recharger_Matrix_Perspective = false;
		     }

        if(not warped.empty())
     {
        cv::circle(warped, cv::Point(WIDTH_WINDOW/2, HEIGHT_WINDOW), 5, cv::Scalar(30,20,255), -1, 200, 0);
        pthread_mutex_lock( &lock );
        MousePick(warped, binary, Positions_Display_CD, paramonmouse, 2, Positions_Display_LINGOT, direct_CDhsv, direct_Lhsv, direct_Nhsv, Positions_Display_N);
        pthread_mutex_unlock( &lock );
        }
		// Handle pause/unpause and ESC
		int c = cv::waitKey( 100 );
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
     fsm2.release();
}
	// Ne pas oublier le wait final
	cm->waitHere();

	return 0;
}
