#include "ColorPicker.h"


Mat colorPickerImage_;
int cpH, cpS, cpV; // HSV pur
int cpHm, cpSm, cpVm; // HSV avec une moyenne par rapport aux derniers clics

void ColorPickerUpdateFrame(Mat &img)
{
	colorPickerImage_ = img.clone();
	imshow("ColorPicker", colorPickerImage_);
}

void ColorPickerResetAverage()
{
	cpHm = cpSm = cpVm = 0;
}

void ColorPickerOnMouse( int event, int x, int y, int, void* )
{

    switch( event )
    {
        case CV_EVENT_LBUTTONDOWN:
            Mat hsv;
            cvtColor(colorPickerImage_, hsv, CV_BGR2HSV);
            Vec3b hsv_selected = hsv.at<Vec3b>(y, x);

            cpH = hsv_selected[0];
            cpS = hsv_selected[1];
            cpV = hsv_selected[2];

            if (!cpHm && !cpSm && !cpVm) {
            	cpHm = cpH;
            	cpSm = cpS;
            	cpVm = cpV;
            } else {
            	cpHm = (cpH + cpHm) / 2;
            	cpSm = (cpS + cpSm) / 2;
            	cpVm = (cpV + cpVm) / 2;
            }

            printf("ColorPicker: h=%d, s=%d, v=%d\n", cpH, cpS, cpV);
            printf("    average: h=%d, s=%d, v=%d\n", cpHm, cpSm, cpVm);
            break;
    }
}

void ColorPickerInit()
{
	namedWindow( "ColorPicker", 1);
	setMouseCallback( "ColorPicker", ColorPickerOnMouse, 0 );
}
