 #include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>
#include <iostream>

using namespace std;

int main ()
{
	int tolerance =20;
	int h,s,v,h2,s2,v2;
	cv::VideoCapture capture(0);
	if (!capture.isOpened())
	{
		cerr << "Failed to open a video device or video file!\n" << endl;
		return 0;
	}
	
	cv::Scalar hsv1, hsv2;	
	cv::Mat image;
	cv::Mat image_hsv;
	cv::Mat warped;
	cv::Mat Binary;
	

	cv::namedWindow( "Raw" , CV_WINDOW_AUTOSIZE);
	cv::namedWindow( "Warped" , CV_WINDOW_AUTOSIZE);
	cv::namedWindow( "Binary" , CV_WINDOW_AUTOSIZE);
	
	
	//cv::setTrackbarPos("h", "Raw", H);
	
	
	while (1)
	{
		capture >> image;
		if (image.empty())
		{
			cerr << "image empty" << endl;
			continue;
		}
		
		
		// Show raw_image
		cv::createTrackbar( "h", "Warped", &(h), 255 );
	cv::createTrackbar( "s", "Warped", &(s), 255 );
	cv::createTrackbar( "h2", "Warped", &(h2), 255 );
	cv::createTrackbar( "s2", "Warped", &(s2), 255 );
	v =130 ;
	v2 = 255 ;
	hsv1[0] = h;
    hsv1[1] = s;
    hsv1[2] = v;
    hsv2[0] = h + tolerance;
    hsv2[1] = s + tolerance;
    hsv2[2] = v2;	
		cv::cvtColor(image, image_hsv, CV_BGR2HSV);
		cv::inRange(image_hsv, hsv1, hsv2, Binary);
		cv::imshow( "Binary", Binary);
		cv::imshow( "Raw", image );
		// Handle pause/unpause and ESC
		int c = cv::waitKey( 15 );
		switch (c)
		{
				case 27:
				return 0;

			default:
				if (c != -1)
					cout << c << endl;
		}
	}
		
	return 0;
	
}

