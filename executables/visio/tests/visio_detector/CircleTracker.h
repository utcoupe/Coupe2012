#ifndef DEF_CircleTracker
#define DEF_CircleTracker


using namespace cv;

int dp = 3; // 2 ou 3
int minDist = 2;
int param1 = 30;
int param2 = 100;
int minRadius = 12;
int maxRadius = 50;

void circleTrackerInit()
{
    namedWindow( "HoughCircles", CV_WINDOW_AUTOSIZE);
    createTrackbar( "dp", "HoughCircles", &dp, 4);
    createTrackbar( "minDist", "HoughCircles", &minDist, 400);
    createTrackbar( "param1", "HoughCircles", &param1, 400);
    createTrackbar( "param2", "HoughCircles", &param2, 400);
    createTrackbar( "minRadius", "HoughCircles", &minRadius, 400);
    createTrackbar( "maxRadius", "HoughCircles", &maxRadius, 400);
}
       
void circleTracker(Mat &rotated)
{
	Mat gray;
    vector<Vec3f> circles;
    cvtColor(rotated, gray, CV_BGR2GRAY);
    GaussianBlur( gray, gray, Size(9, 9), 2, 2 );
    Canny( gray, gray, 50, 200, 3 );
    //GaussianBlur( gray, gray, Size(9, 9), 2, 2 );
    imshow("canny", gray);
    HoughCircles(gray, circles, CV_HOUGH_GRADIENT, dp, minDist, param1, param2, minRadius, maxRadius);
    for( size_t i = 0; i < circles.size(); i++ )
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        // draw the circle center
        circle( rotated, center, 3, Scalar(0,255,0), -1, 8, 0 );
        // draw the circle outline
        circle( rotated, center, radius, Scalar(0,0,255), 3, 8, 0 );
    }
}

#endif