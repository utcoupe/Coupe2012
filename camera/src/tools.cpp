#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>
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

