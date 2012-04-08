#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>
#include "../include/detection.h"
#include <iostream>
#include "../include/tools.h"
#include "../include/parametres.h"

using namespace std;

cv::Point barycentre (vector<cv::Point> & contour)
{
	int summ_x, summ_y, bar_x, bar_y;
	summ_x = summ_y = bar_x = bar_y = 0;
	for (unsigned int i=0; i < contour.size(); i++)
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

		int h, s, v, h2, v2, s2;
        h = parametre->hsv_selected[0] - HSV_TOLERANCE1;
		s = parametre->hsv_selected[1] - HSV_TOLERANCE2;
		v = parametre->hsv_selected[2] -  HSV_TOLERANCE3;
		h2 = parametre->hsv_selected[0] + HSV_TOLERANCE1;
		s2 = parametre->hsv_selected[1] + HSV_TOLERANCE2;
		v2 = parametre->hsv_selected[2] +  HSV_TOLERANCE3;
/*      cout << "h: " <<  (int)h<< endl;
        cout << "s: " <<  (int)s<< endl;
        cout << "v: " <<  (int)v<< endl;
        cout << "h2: " <<  (int)h2<< endl;
        cout << "s2: " <<  (int)s2<< endl;
        cout << "v2: " <<  (int)v2<< endl;
*/
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

        cv::Point2f pt(x,y);
        pt = px2mm(pt);
        cout << "Coordonnee en mm" << endl <<" x: " <<pt.x << " y: " <<pt.y << endl;

        DeterminateHSV(paramonmouse);

        break;

    }
}


