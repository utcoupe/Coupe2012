#ifndef DEF_FPS
#define DEF_FPS


#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <time.h>
#include <stdio.h>

using namespace cv;

class FPS {
    // start and end times
    time_t start, end;
    // frame counter
    int counter;
    // floating point seconds elapsed since start
    double sec;
    // start the clock

public:
    // fps calculated using number of frames / seconds
    double fps;

    FPS();
    double tic();
    void draw(Mat &img);
};

#endif