#include "FPS.h"


FPS::FPS()
{
    counter = 0;
    time(&start);
}

double FPS::tic()
{
    // see how much time has elapsed
    time(&end);
    // calculate current FPS
    ++counter;
    sec = difftime (end, start);
    if (sec >= 1)
    {
        fps = counter / sec;
        counter = 0;
        time(&start);
    }
    return fps;
}

void FPS::draw(Mat &img)
{
    char notice[200];
    sprintf(notice, "FPS = %.2f", fps);
    putText(img, notice, Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(255, 255, 255));
}