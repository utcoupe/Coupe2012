#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>
#include <cmath>
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

void findObjects(cv::Mat src, vector<cv::Point>& VecOfPosition, int indice_objet) {

    vector<cv::Point> result;
    vector<vector<cv::Point> > contours;
    vector<cv::Vec4i> hierarchy;

    cv::findContours( src, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );

//    result.resize(contours.size());

    for (unsigned int i=0; i < contours.size(); i++)
    {
        cv::Point bary;
        vector<cv::Point> contour = contours[i];
        bary = barycentre(contour);
        if (not EliminatedContour(contour, bary, indice_objet))
        {
        result.push_back(bary);
        }
    }
    VecOfPosition = result;
}

void MousePick(cv::Mat& warped, cv::Mat& binary, const vector<cv::Point>& Positions_Display_CD,
               ParamonMouse& paramonmouse, int index, const vector<cv::Point>& Positions_Display_LINGOT)
{
        cv::Mat warped_hsv;
        paramonmouse.image = &warped;


		cvSetMouseCallback( "Warped", onMouse, &paramonmouse);

		cv::cvtColor(warped, warped_hsv, CV_BGR2HSV);
		cv::GaussianBlur(warped_hsv, warped_hsv, cv::Size (9, 9), 2, 2);

        if (index == 0)
            cv::inRange(warped_hsv, cv::Scalar(CD_MIN_H, CD_MIN_S, CD_MIN_V), cv::Scalar(CD_MAX_H, CD_MAX_S, CD_MAX_V), binary);
        else if (index == 1)
            cv::inRange(warped_hsv, cv::Scalar(LINGOT_MIN_H, LINGOT_MIN_S, LINGOT_MIN_V), cv::Scalar(LINGOT_MAX_H, LINGOT_MAX_S, LINGOT_MAX_V), binary);
        else
		    cv::inRange(warped_hsv, paramonmouse.hsv1, paramonmouse.hsv2, binary);

		cv::GaussianBlur(binary, binary, cv::Size (9, 9), 2, 2);

        for (unsigned int i=0; i < Positions_Display_CD.size(); i++) {
        cv::circle(warped, Positions_Display_CD[i], 3, cv::Scalar(255,0,0), -1, 200, 0);
        }
        for (unsigned int i=0; i < Positions_Display_LINGOT.size(); i++) {
        cv::circle(warped, Positions_Display_LINGOT[i], 3, cv::Scalar(0,255,0), -1, 200, 0);
        }
		cv::imshow( "Warped", warped );
}

void MousePick(cv::Mat& warped, cv::Mat& binary, const vector<cv::Point>& Positions_Display,
               ParamonMouse& paramonmouse, int index)
{
        cv::Mat warped_hsv;
        paramonmouse.image = &warped;


		cvSetMouseCallback( "Warped", onMouse, &paramonmouse);

		cv::cvtColor(warped, warped_hsv, CV_BGR2HSV);
		cv::GaussianBlur(warped_hsv, warped_hsv, cv::Size (9, 9), 2, 2);

          int CD_MIN_H = CD_H - HSV_TOLERANCE1;
          int CD_MIN_S = CD_S - HSV_TOLERANCE2;
		int CD_MIN_V = CD_V - HSV_TOLERANCE3;

		int CD_MAX_H = CD_H + HSV_TOLERANCE1;
		int CD_MAX_S = CD_S + HSV_TOLERANCE2;
		int CD_MAX_V = CD_V + HSV_TOLERANCE3;

          int LINGOT_MIN_H = LINGOT_H - HSV_TOLERANCE1;
		int LINGOT_MIN_S = LINGOT_S - HSV_TOLERANCE2;
		int LINGOT_MIN_V = LINGOT_V - HSV_TOLERANCE3;

		int LINGOT_MAX_H = LINGOT_H + HSV_TOLERANCE1;
		int LINGOT_MAX_S = LINGOT_S + HSV_TOLERANCE2;
		int LINGOT_MAX_V = LINGOT_V + HSV_TOLERANCE3;

        if (index == 0)
            cv::inRange(warped_hsv, cv::Scalar(CD_MIN_H, CD_MIN_S, CD_MIN_V), cv::Scalar(CD_MAX_H, CD_MAX_S, CD_MAX_V), binary);
        else if (index == 1)
            cv::inRange(warped_hsv, cv::Scalar(LINGOT_MIN_H, LINGOT_MIN_S, LINGOT_MIN_V), cv::Scalar(LINGOT_MAX_H, LINGOT_MAX_S, LINGOT_MAX_V), binary);
        else
		    cv::inRange(warped_hsv, paramonmouse.hsv1, paramonmouse.hsv2, binary);

		cv::GaussianBlur(binary, binary, cv::Size (9, 9), 2, 2);

		cv::imshow( "Warped", warped );
}

bool EliminatedContour(vector<cv::Point> contour, cv::Point bary, int index)
{
    bary = px2mm(bary);
    px2mm(contour);


    cv::RotatedRect MinRect;
    unsigned int Norm = 0;
    for (unsigned int i=0; i<contour.size(); i++)
    {
            Norm += sqrt ( pow( (contour[i].x - bary.x), 2) + pow( (contour[i].y - bary.y), 2) );
    }
    Norm = Norm/contour.size();


    if (index==0)
    {
        if (Norm >= TOLERANCE_MIN_CD and Norm <= TOLERANCE_MAX_CD)
        return false;
        else return true;
    }
    else
    {
        if (Norm >= TOLERANCE_MIN_LINGOT and Norm <= TOLERANCE_MAX_LINGOT)
        return false;
        else return true;
    }

/*  float radius;
    cv::Point2f bary2f = bary;
    if (index==0)
    {
        cv::minEnclosingCircle(cv::Mat(contour), bary2f, radius);
        if (radius >= TOLERANCE_MIN_CD and radius <= TOLERANCE_MAX_CD)
        {
            cout<<"raduis est de: "<<radius<<endl;
            return false;
        }
        else return true;
    }
    else
    {
        MinRect = minAreaRect(cv::Mat(contour));

        if ((MinRect.size.height >= TOLERANCE_MIN_LINGOT_H and MinRect.size.height <= TOLERANCE_MAX_LINGOT_H)
            and (MinRect.size.width >= TOLERANCE_MIN_LINGOT_W and MinRect.size.width <= TOLERANCE_MAX_LINGOT_W)   )
        {
            cout<<"Height: "<<MinRect.size.height<<"Width: "<<MinRect.size.width<<endl;
            return false;
        }
        else return true;
    }*/
}















