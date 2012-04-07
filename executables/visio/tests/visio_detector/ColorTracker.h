#ifndef DEF_ColorTracker
#define DEF_ColorTracker


using namespace cv;

int h,s,v;
int h2,s2,v2;
int tolerance = 10;
int epsilon;
int taille_object = 0;

void colorTrackerInit()
{
	h=s=v=0;
	h2=s2=v2=254;

    // angry bird rouge
    h=130;
    s=130;
    h2=200;
    s2=255;
    // lingot jaune
    h=20;
    s=233;
    v=50;
    h2=43;
    s2=255;
    v2=255;
	
    createTrackbar( "h", "One", &h, 255 );
    createTrackbar( "s", "One", &s, 255 );
    createTrackbar( "v", "One", &v, 255 );
    createTrackbar( "h2", "One", &h2, 255 );
    createTrackbar( "s2", "One", &s2, 255 );
    createTrackbar( "v2", "One", &v2, 255 );
    createTrackbar( "epsilon", "One", &epsilon, 2500 );
    createTrackbar( "taille objet", "One", &taille_object, 1000 );
}

void colorTracker(Mat &rotated, Perspective &pers)
{ 
	Mat hsv, hsv_range_result, worker_contours, result_contours, matlines;

	// Filtre HSV
	cvtColor(rotated, hsv, CV_BGR2HSV);
	inRange(hsv, Scalar(h, s, v), Scalar(h2, s2, v2), hsv_range_result);
	//imshow("hsv", hsv_range_result);

	// Recherche des contours
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	worker_contours = hsv_range_result.clone();
	worker_contours = worker_contours > 1;
	GaussianBlur(hsv_range_result, matlines, Size(9, 9), 2, 2);
	findContours( matlines, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
	int idx = 0;
	if (contours.size() > 0)
	{
		for( ; idx >= 0; idx = hierarchy[idx][0] )
		{
			if (contours[idx].size() < taille_object) continue;
			vector<Point> approx;
			drawContours( rotated, contours, idx, Scalar(0, 255, 0), CV_FILLED, 8, hierarchy );
			//puts("drawContours");
			approxPolyDP(Mat(contours[idx]), approx, epsilon, false);
			if( fabs(contourArea(Mat(approx))) > 100 )
			{
				Scalar color( rand()&255, rand()&255, rand()&255 );
				const Point* p = &approx[0];
				int n = (int)approx.size();
				polylines(rotated, &p, &n, 1, true, color, 3, CV_AA);

				// Trouve le centre d'un polygone et le trace d'un cercle
				Moments mo = moments(approx, false);
				Point objectCenter(mo.m10/mo.m00, mo.m01/mo.m00);
				circle( rotated, objectCenter, 3, Scalar(0,0,255), 3, 8, 0 );
				CvPoint2D32f realPosition = pers.realDistanceToChessOrigin(objectCenter);

				// Affiche le position du point sur l'image
				char notice[200];
				sprintf(notice, "%.1f, %.1f", realPosition.x, realPosition.y);
				putText(rotated, notice, objectCenter, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255));
			}
		}
	}
}

#endif
