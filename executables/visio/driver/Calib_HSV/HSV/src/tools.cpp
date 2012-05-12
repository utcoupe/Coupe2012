#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>
#include <fstream>
#include "../include/parametres.h"

using namespace std;

cv::Point2f px2mm(cv::Point2f & p)
{
    p.x = p.x * RATIO_X;
    p.y = p.y * RATIO_Y;

    return p;
}

cv::Point2f px2mm(cv::Point & p)
{
    p.x = p.x * RATIO_X;
    p.y = p.y * RATIO_Y;

    return p;
}

cv::Point2f mm2px(cv::Point & p)
{
    p.x = p.x * RATIOX_MM2PX;
    p.y = p.y * RATIOY_MM2PX;
    return p;
}

void px2Cam(cv::Point & p)
{
     p.x = p.x - WIDTH_WINDOW/2;
     p.y = HEIGHT_WINDOW - p.y;
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
    for (unsigned int i = 0; i < p.size(); i++)
    {
        p[i].x = p[i].x * RATIO_X;
        p[i].y = p[i].y * RATIO_Y;
    }
}

bool fexists(const char *filename)
{
  ifstream ifile(filename);
  return ifile;
}







