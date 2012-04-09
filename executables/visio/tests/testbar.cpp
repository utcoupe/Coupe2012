/**
 * Test de la fonction barycentre
 *
 */


#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>
#include <iostream>
using namespace std;

cv::Point barycentre (vector<cv::Point> & contour)
{
	int summ_x, summ_y, bar_x, bar_y;
	summ_x = summ_y = bar_x = bar_y = 0;
	for (int i=0; i < contour.size(); i++)
	{	
		summ_x += contour[i].x;
		summ_y += contour[i].y;
	}

	bar_x = (summ_x/(contour.size()/2));
	bar_y = (summ_y/(contour.size()/2));
	cv::Point bar(bar_x, bar_y);
	return bar;
}

int main ()
{
	
int bary_y,bary_x;
cv::Point bary;
vector<cv::Point> contour;
contour.push_back(cv::Point( 1, 4));
contour.push_back(cv::Point( 2, 6));
contour.push_back(cv::Point( 3, 8));
contour.push_back(cv::Point( 4, 2));

bary = barycentre(contour);

cout << bary.x << endl;
cout << bary.y << endl;
return  0;

}


