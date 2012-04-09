#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>
#include <iostream>

using namespace std;


typedef struct
{
	cv::Scalar hsv1, hsv2;
	int h,s,v,h2,s2,v2;
	cv::Mat * image;
	cv::Vec3b hsv_selected;
} ParamonMouse;

cv::Vec3b hsv_selected;
cv::Scalar hsv_pixel;
bool hsv_selected_ready = false;

// fonction barycentre
cv::Point barycentre (vector<cv::Point> & contour)
{
	int summ_x, summ_y, bar_x, bar_y;
	summ_x = summ_y = bar_x = bar_y = 0;
	for (int i=0; i < contour.size(); i++)
	{	
		summ_x += contour[i].x;
		summ_y += contour[i].y;
	}
	//cout << contour.size()<< endl;

	bar_x = (summ_x/(contour.size()));
	bar_y = (summ_y/(contour.size()));
	cv::Point bar(bar_x, bar_y);
	return bar;
}

void DeterminateHSV (ParamonMouse *parametre)
{
		int tolerance = 10;
		int tolerance2 = 40;
		int tolerance3 = 80;
		int h, s, v, h2, v2, s2;
        h = parametre->hsv_selected[0] - tolerance;
		s = parametre->hsv_selected[1] - tolerance2;
		v = parametre->hsv_selected[2] -  tolerance3;
		h2 = parametre->hsv_selected[0] + tolerance;
		s2 = parametre->hsv_selected[1] + tolerance2;
		v2 = parametre->hsv_selected[2] +  tolerance3; 
		cout << "h: " <<  (int)h<< endl;
        cout << "s: " <<  (int)s<< endl;
        cout << "v: " <<  (int)v<< endl;
        cout << "h2: " <<  (int)h2<< endl;
        cout << "s2: " <<  (int)s2<< endl;
        cout << "v2: " <<  (int)v2<< endl;
        
        parametre->hsv1[0] = h;
        parametre->hsv1[1] = s;
        parametre->hsv1[2] = v;
        parametre->hsv2[0] = h2;
        parametre->hsv2[1] = s2;
        parametre->hsv2[2] = v2;
}
	
void onMouse(int event, int x, int y, int flags, void * param)
{
	ParamonMouse * paramonmouse = (ParamonMouse*) param;
    switch( event )
    {
    case CV_EVENT_LBUTTONDOWN:
        cv::Mat hsv;
        cv::cvtColor(*(paramonmouse->image), hsv, CV_BGR2HSV);
        paramonmouse->hsv_selected = hsv.at<cv::Vec3b>(y, x);
        cout << "hsv_selected: " <<  (int)paramonmouse->hsv_selected[0]<< endl;
        cout << "hsv_selected: " <<  (int)paramonmouse->hsv_selected[1]<< endl;
        cout << "hsv_selected: " <<  (int)paramonmouse->hsv_selected[2]<< endl;
        DeterminateHSV(paramonmouse);
	
        break;
      
    }
}
int main ()
{
	
	ParamonMouse paramonmouse;
	cv::VideoCapture capture(0);
	if (!capture.isOpened())
	{
		cerr << "Failed to open a video device or video file!\n" << endl;
		return 0;
	}
	
	cv::Mat image;
	cv::Mat image_hsv;
	cv::Mat warped;
	cv::Mat binary;
	
	paramonmouse.image = &image;

	cv::namedWindow( "Raw" , CV_WINDOW_AUTOSIZE);
	cvSetMouseCallback( "Raw", onMouse, &paramonmouse);
	cv::namedWindow( "Binary" , CV_WINDOW_AUTOSIZE);
	
	/*cv::createTrackbar( "h", "Raw", &(paramonmouse.h), 255 );
	cv::createTrackbar( "s", "Raw", &(paramonmouse.s), 255 );
	cv::createTrackbar( "v", "Raw", &(paramonmouse.v), 255 );*/
	
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
		cv::cvtColor(image, image_hsv, CV_BGR2HSV);
		cv::GaussianBlur(image_hsv, image_hsv, cv::Size (9, 9), 3, 3);
		cv::inRange(image_hsv, paramonmouse.hsv1, paramonmouse.hsv2, binary);
		cv::imshow( "binary", binary);
		
		
		vector<vector<cv::Point> > contours;
		vector<cv::Vec4i> hierarchy;
		
		cv::findContours( binary, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );
		if (contours.size() > 0)
		{
			
			//image3 = image.clone();
			cv::drawContours( image, contours, -1, cv::Scalar(0,0,255), 2 );
		}
		vector<cv::Point> contour;
		for (int i=0; i < contours.size(); i++)
		{
		cv::Point bary;
		contour = contours[i];
		bary = barycentre(contour);
		// affiche le barycentre
		cv::circle(image, bary, 3, cv::Scalar(0,255,8), -1, 200, 0);
		}

		// calcul des barycentres
		
		//cv::imshow( "Three", image );
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

