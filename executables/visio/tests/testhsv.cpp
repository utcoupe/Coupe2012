#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>
#include <iostream>

using namespace std;

typedef struct
{
	int hsv[3];
	bool update_hsv;
	const cv::Mat * image;
	
} paramOnMouse;

void onMouse(int event, int x, int y, int flags, void * param)
{
	paramOnMouse * p = (paramOnMouse*) param;

	switch( event )
	{
		case CV_EVENT_LBUTTONDOWN:
			cv::Mat hsv;
			cv::cvtColor(*(p->image), hsv, CV_BGR2HSV);
			cv::Vec3b hsv_selected = hsv.at<cv::Vec3b>(y, x);
			for (int i=0; i<3; ++i)
			{
				p->hsv[i] = hsv_selected[i];
				cout << p->hsv[i] << " ";
			}
			cout << endl;
			p->update_hsv = true;
		break;
	}
}


int main(int ac, char** av)
{
	cv::VideoCapture capture(0);
	if (!capture.isOpened())
	{
		cerr << "Failed to open a video device or video file!\n" << endl;
		return 0;
	}
	cv::Mat image;
	/*
	const cv::Mat image = cv::imread("monimage.png", 1);
	if (image.empty())
	{
		cerr << "image vide" << endl;
		return 0;
	}*/
    
	cv::Mat image2, image3, temp;
	int h1,s1,v1, h2,s2,v2;
	paramOnMouse param;
	param.image = &image;
	param.update_hsv = false;

	cv::namedWindow( "One" , CV_WINDOW_AUTOSIZE);
	cv::namedWindow( "Two" , CV_WINDOW_AUTOSIZE);
	cv::namedWindow( "Three" , CV_WINDOW_AUTOSIZE);
	cvSetMouseCallback("One", onMouse, &param);
	cv::createTrackbar( "h1", "One", &h1, 255 );
	cv::createTrackbar( "s1", "One", &s1, 255 );
	cv::createTrackbar( "v1", "One", &v1, 255 );
	cv::createTrackbar( "h2", "One", &h2, 255 );
	cv::createTrackbar( "s2", "One", &s2, 255 );
	cv::createTrackbar( "v2", "One", &v2, 255 );
	cv::setTrackbarPos( "h1", "One", 0 );
	cv::setTrackbarPos( "s1", "One", 0 );
	cv::setTrackbarPos( "v1", "One", 130 );
	cv::setTrackbarPos( "h2", "One", 30 );
	cv::setTrackbarPos( "s2", "One", 100 );
	cv::setTrackbarPos( "v2", "One", 255 );

	
	while(1)
	{
		capture >> image;
		
		// affichage image de base
		//cv::imshow( "One", image );

		// flou gaussien pour éliminer les bruits
		cv::GaussianBlur( image, temp, cv::Size(9, 9), 4, 4 );

		// convertion en hsv
		cv::cvtColor(temp, temp, CV_BGR2HSV);

		// selection des couleurs qui nous interessent
		cv::inRange(temp, cv::Scalar(h1,s1,v1), cv::Scalar(h2,s2,v2), image2);

		// affichage du résultat de la selection
		cv::imshow( "Two", image2);

		
		vector<vector<cv::Point> > contours;
		vector<cv::Vec4i> hierarchy;
		
		cv::findContours( image2, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );
		if (contours.size() > 0)
		{
			//image3 = image.clone();
			cv::drawContours( image, contours, -1, cv::Scalar(0,0,255), 2 );
		}
		cv::imshow( "Three", image );


		if (param.update_hsv)
		{
			param.update_hsv = false;
			h1 = param.hsv[0] - 20;
			s1 = param.hsv[1] - 50;
			v1 = param.hsv[2] - 70;
			h2 = param.hsv[0] + 20;
			s2 = param.hsv[1] + 50;
			v2 = param.hsv[2] + 70;
			cv::setTrackbarPos( "h1", "One", h1 );
			cv::setTrackbarPos( "s1", "One", s1 );
			cv::setTrackbarPos( "v1", "One", v1 );
			cv::setTrackbarPos( "h2", "One", h2 );
			cv::setTrackbarPos( "s2", "One", s2 );
			cv::setTrackbarPos( "v2", "One", v2 );
		}
		
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


