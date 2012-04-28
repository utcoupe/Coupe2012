#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>
#include <fstream>
#include "../include/parametres.h"

using namespace std;

cv::Point2f px2mm(cv::Point2f & p)
{
    float ratioX, ratioY;
    ratioX = BOARD_HEIGHT_MM/BOARD_HEIGHT_PX;
    ratioY = BOARD_WIDTH_MM/BOARD_WIDTH_PX;

    p.x = p.x * ratioX;
    p.y = p.y * ratioY;

    return p;
}

cv::Point2f px2mm(cv::Point & p)
{
    float ratioX, ratioY;
    ratioX = BOARD_HEIGHT_MM/BOARD_HEIGHT_PX;
    ratioY = BOARD_WIDTH_MM/BOARD_WIDTH_PX;

    p.x = p.x * ratioX;
    p.y = p.y * ratioY;

    return p;
}

void id()
{
    cout<<"Visio";
}

void ping()
{
    cout<<"pong";
}

void px2mm(vector<cv::Point> & p)
{
    float ratioX, ratioY;
    ratioX = BOARD_HEIGHT_MM/BOARD_HEIGHT_PX;
    ratioY = BOARD_WIDTH_MM/BOARD_WIDTH_PX;

    for (unsigned int i = 0; i < p.size(); i++)
    {
        p[i].x = p[i].x * ratioX;
        p[i].y = p[i].y * ratioY;
    }
}

bool fexists(const char *filename)
{
  ifstream ifile(filename);
  return ifile;
}

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
}



