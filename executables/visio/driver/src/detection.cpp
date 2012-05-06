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
        cerr << "hsv_selected: " <<  (int)paramonmouse->hsv_selected[0]<< endl;
        cerr << "hsv_selected: " <<  (int)paramonmouse->hsv_selected[1]<< endl;
        cerr << "hsv_selected: " <<  (int)paramonmouse->hsv_selected[2]<< endl;

        cv::Point pt(x,y);
        px2Cam(pt);
        //Cam2CR(pt);
        cerr << "Coordonnee en px" << endl <<" x: " <<pt.x << " y: " <<pt.y << endl;
        px2mm(pt);
        cerr << "Coordonnee en mm" << endl <<" x: " <<pt.x << " y: " <<pt.y << endl;
        DeterminateHSV(paramonmouse);

        break;

    }
}

void findObjects(cv::Mat src, vector<cv::Point>& VecOfPosition, int indice_objet) {

    vector<cv::Point> result;
    vector<vector<cv::Point> > contours;
    vector<cv::Vec4i> hierarchy;

    cv::findContours( src, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );

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
               ParamonMouse& paramonmouse, int index, const vector<cv::Point>& Positions_Display_LINGOT,
               string direct_CDhsv, string direct_Lhsv, string direct_Nhsv, const vector<cv::Point>& Positions_Display_N)
{
        cv::Mat warped_hsv;
        paramonmouse.image = &warped;


		cvSetMouseCallback( "Warped", onMouse, &paramonmouse);

		cv::cvtColor(warped, warped_hsv, CV_BGR2HSV);
		cv::GaussianBlur(warped_hsv, warped_hsv, cv::Size (9, 9), 2, 2);

               int h_cd, s_cd, v_cd, h2_cd,s2_cd,v2_cd;
               int h_L, s_L, v_L, h2_L, s2_L, v2_L;
               int h_N, s_N, v_N, h2_N, s2_N, v2_N;
               if(fexists(direct_CDhsv.c_str()) and fexists(direct_Lhsv.c_str()) and fexists(direct_Nhsv.c_str()) )
               {
                    cv::FileStorage fCD(direct_CDhsv, cv::FileStorage::READ);
                    fCD["h_cd"] >> h_cd;
                    fCD["s_cd"] >> s_cd;
                    fCD["v_cd"] >> v_cd;
                    fCD["h2_cd"] >> h2_cd;
                    fCD["s2_cd"] >> s2_cd;
                    fCD["v2_cd"] >> v2_cd;
                    cv::FileStorage fL(direct_Lhsv, cv::FileStorage::READ);
                    fL["h_L"] >> h_L;
                    fL["s_L"] >> s_L;
                    fL["v_L"] >> v_L;
                    fL["h2_L"] >> h2_L;
                    fL["s2_L"] >> s2_L;
                    fL["v2_L"] >> v2_L;
                    cv::FileStorage fN(direct_Nhsv, cv::FileStorage::READ);
                    fN["h_N"] >> h_N;
                    fN["s_N"] >> s_N;
                    fN["v_N"] >> v_N;
                    fN["h2_N"] >> h2_N;
                    fN["s2_N"] >> s2_N;
                    fN["v2_N"] >> v2_N;

                    fCD.release();
                    fL.release();
                    fN.release();
               }
               else
               cerr << "Failed to open hsv config!" << endl;

        switch (index) {
          case 0: cv::inRange(warped_hsv, cv::Scalar(h_cd, s_cd, v_cd), cv::Scalar(h2_cd, s2_cd, v2_cd), binary); break;
          case 1: cv::inRange(warped_hsv, cv::Scalar(h_L, s_L, v_L), cv::Scalar(h2_L, s2_L, v2_L), binary); break;
          case 2: cv::inRange(warped_hsv, cv::Scalar(h_N, s_N, v_N), cv::Scalar(h2_N, s2_N, v2_N), binary); break;
        default : cv::inRange(warped_hsv, paramonmouse.hsv1, paramonmouse.hsv2, binary);
        }
		cv::GaussianBlur(binary, binary, cv::Size (9, 9), 2, 2);

        for (unsigned int i=0; i < Positions_Display_CD.size(); i++) {
        cv::circle(warped, Positions_Display_CD[i], 3, cv::Scalar(255,0,0), -1, 200, 0);
        }
        for (unsigned int i=0; i < Positions_Display_LINGOT.size(); i++) {
        cv::circle(warped, Positions_Display_LINGOT[i], 3, cv::Scalar(0,255,0), -1, 200, 0);
        }
        for (unsigned int i=0; i < Positions_Display_N.size(); i++) {
        cv::circle(warped, Positions_Display_N[i], 3, cv::Scalar(14,124,255), -1, 200, 0);
        }

        if(ActivationVideo) {
		cv::imshow( "Warped", warped );}
}

void MousePick(cv::Mat& warped, cv::Mat& binary, const vector<cv::Point>& Positions_Display,
               ParamonMouse& paramonmouse, int index, string direct_CDhsv, string direct_Lhsv, string direct_Nhsv)
{
        cv::Mat warped_hsv;
        paramonmouse.image = &warped;


		cvSetMouseCallback( "Warped", onMouse, &paramonmouse);

		cv::cvtColor(warped, warped_hsv, CV_BGR2HSV);
		cv::GaussianBlur(warped_hsv, warped_hsv, cv::Size (9, 9), 2, 2);

               int h_cd, s_cd, v_cd, h2_cd,s2_cd,v2_cd;
               int h_L, s_L, v_L, h2_L, s2_L, v2_L;
               int h_N, s_N, v_N, h2_N, s2_N, v2_N;
                              if(fexists(direct_CDhsv.c_str()) and fexists(direct_Lhsv.c_str()) and fexists(direct_Nhsv.c_str()) )
               {
                    cv::FileStorage fCD(direct_CDhsv, cv::FileStorage::READ);
                    fCD["h_cd"] >> h_cd;
                    fCD["s_cd"] >> s_cd;
                    fCD["v_cd"] >> v_cd;
                    fCD["h2_cd"] >> h2_cd;
                    fCD["s2_cd"] >> s2_cd;
                    fCD["v2_cd"] >> v2_cd;
                    cv::FileStorage fL(direct_Lhsv, cv::FileStorage::READ);
                    fL["h_L"] >> h_L;
                    fL["s_L"] >> s_L;
                    fL["v_L"] >> v_L;
                    fL["h2_L"] >> h2_L;
                    fL["s2_L"] >> s2_L;
                    fL["v2_L"] >> v2_L;
                    cv::FileStorage fN(direct_Nhsv, cv::FileStorage::READ);
                    fN["h_N"] >> h_N;
                    fN["s_N"] >> s_N;
                    fN["v_N"] >> v_N;
                    fN["h2_N"] >> h2_N;
                    fN["s2_N"] >> s2_N;
                    fN["v2_N"] >> v2_N;

                    fCD.release();
                    fL.release();
                    fN.release();
               }
               else
               cerr << "Failed to open hsv config!" << endl;

        switch (index) {
          case 0: cv::inRange(warped_hsv, cv::Scalar(h_cd, s_cd, v_cd), cv::Scalar(h2_cd, s2_cd, v2_cd), binary); break;
          case 1: cv::inRange(warped_hsv, cv::Scalar(h_L, s_L, v_L), cv::Scalar(h2_L, s2_L, v2_L), binary); break;
          case 2: cv::inRange(warped_hsv, cv::Scalar(h_N, s_N, v_N), cv::Scalar(h2_N, s2_N, v2_N), binary); break;
        default : cv::inRange(warped_hsv, paramonmouse.hsv1, paramonmouse.hsv2, binary);
}
		cv::GaussianBlur(binary, binary, cv::Size (9, 9), 2, 2);

if(ActivationVideo) {
		cv::imshow( "Warped", warped );}
}

bool EliminatedContour(vector<cv::Point> contour, cv::Point bary, int index)
{
    bary = px2mm(bary);
    px2mm(contour);

    float radius;
    cv::Point2f bary2f = bary;

    unsigned int Norm = 0;
    for (unsigned int i=0; i<contour.size(); i++)
    {
            Norm += sqrt ( pow( (contour[i].x - bary.x), 2) + pow( (contour[i].y - bary.y), 2) );
    }
    Norm = Norm/contour.size();
    cerr<<index<<"Norm= "<<Norm<<endl;

    switch (index) {
         case 0:
              cv::minEnclosingCircle(cv::Mat(contour), bary2f, radius);
              cerr<<"raduis B est de: "<<radius<<endl;
                  if (radius >= TOLERANCE_MIN_CD and radius <= TOLERANCE_MAX_CD)
                      {return false; break;}
                    else {return true; break;}
     /*   cout<<"cd norm: "<<Norm<<endl;
        if (Norm >= TOLERANCE_MIN_CD and Norm <= TOLERANCE_MAX_CD)
        return false;
        else return true;*/
        case 1:
                  cerr<<"lingot norm: "<<Norm<<endl;
                  if (Norm >= TOLERANCE_MIN_LINGOT and Norm <= TOLERANCE_MAX_LINGOT)
                      {return false; break;}
                    else {return true; break;}
        case 2:
              cv::minEnclosingCircle(cv::Mat(contour), bary2f, radius);
              cerr<<"raduis N est de: "<<radius<<endl;
                  if (radius >= TOLERANCE_MIN_CD and radius <= TOLERANCE_MAX_CD)
                      {return false; break;}
                    else {return true; break;}
        default: cerr<<"why are you give me a indice other than 0 1 2 ?"; return true; break;
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















