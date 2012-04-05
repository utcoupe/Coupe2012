#ifndef DEF_ColorPicker
#define DEF_ColorPicker

#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <stdio.h>

using namespace cv;

/**
 * Donne le HSV d'un pixel au clic de souris.
 * Usage:
 * ColorPickerResetAverage();
 * ColorPickerInit();
 * loop{ ColorPickerUpdateFrame(Mat &img); }
 */

void ColorPickerUpdateFrame(Mat &img);
void ColorPickerResetAverage();
void ColorPickerOnMouse( int event, int x, int y, int, void* );
void ColorPickerInit();

#endif
