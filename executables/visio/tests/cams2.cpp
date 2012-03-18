/*
 * Code written by Lya (GeckoGeek.fr)
 */
 
#include "opencv/highgui.h"
#include "opencv/cv.h"
 
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
 
// Maths methods
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))  
#define abs(x) ((x) > 0 ? (x) : -(x))
#define sign(x) ((x) > 0 ? 1 : -1)
 
// Step mooving for object min & max
#define STEP_MIN 5
#define STEP_MAX 100 
 
IplImage *image;
 
// Position of the object we overlay
CvPoint objectPos = cvPoint(-1, -1);
// Color tracked and our tolerance towards it
int h = 0, s = 0, v = 0, tolerance = 10;
 
/*
 * Transform the image into a two colored image, one color for the color we want to track, another color for the others colors
 * From this image, we get two datas : the number of pixel detected, and the center of gravity of these pixel
 */
CvPoint binarisation(IplImage* image, int *nbPixels) {
 
	int x, y;
	CvScalar pixel;
	IplImage *hsv, *mask;
	IplConvKernel *kernel;
	int sommeX = 0, sommeY = 0;
	*nbPixels = 0;
 
	// Create the mask &initialize it to white (no color detected)
	mask = cvCreateImage(cvGetSize(image), image->depth, 1);
 
	// Create the hsv image
	hsv = cvCloneImage(image);
	cvCvtColor(image, hsv, CV_BGR2HSV);
 
	// We create the mask
	cvInRangeS(hsv, cvScalar(h - tolerance -1, s - tolerance, 0), cvScalar(h + tolerance -1, s + tolerance, 255), mask);
 
	// Create kernels for the morphological operation
	kernel = cvCreateStructuringElementEx(5, 5, 2, 2, CV_SHAPE_ELLIPSE);
 
	// Morphological opening (inverse because we have white pixels on black background)
	cvDilate(mask, mask, kernel, 1);
	cvErode(mask, mask, kernel, 1);  
 
	// We go through the mask to look for the tracked object and get its gravity center
	for(x = 0; x < mask->width; x++) {
		for(y = 0; y < mask->height; y++) { 
 
			// If its a tracked pixel, count it to the center of gravity's calcul
			if(((uchar *)(mask->imageData + y*mask->widthStep))[x] == 255) {
				sommeX += x;
				sommeY += y;
				(*nbPixels)++;
			}
		}
	}
 
	// Show the result of the mask image
	cvShowImage("GeckoGeek Mask", mask);
 
	// We release the memory of kernels
	cvReleaseStructuringElement(&kernel);
 
	// We release the memory of the mask
	cvReleaseImage(&mask);
	// We release the memory of the hsv image
    	cvReleaseImage(&hsv);
 
	// If there is no pixel, we return a center outside the image, else we return the center of gravity
	if(*nbPixels > 0)
		return cvPoint((int)(sommeX / (*nbPixels)), (int)(sommeY / (*nbPixels)));
	else
		return cvPoint(-1, -1);
}
 
/*
 * Add a circle on the video that fellow your colored object
 */
void addObjectToVideo(IplImage* image, CvPoint objectNextPos, int nbPixels) {
 
	int objectNextStepX, objectNextStepY;
 
	// Calculate circle next position (if there is enough pixels)
	if (nbPixels > 10) {
 
		// Reset position if no pixel were found
		if (objectPos.x == -1 || objectPos.y == -1) {
			objectPos.x = objectNextPos.x;
			objectPos.y = objectNextPos.y;
		}
 
		// Move step by step the object position to the desired position
		if (abs(objectPos.x - objectNextPos.x) > STEP_MIN) {
			objectNextStepX = max(STEP_MIN, min(STEP_MAX, abs(objectPos.x - objectNextPos.x) / 2));
			objectPos.x += (-1) * sign(objectPos.x - objectNextPos.x) * objectNextStepX;
		}
		if (abs(objectPos.y - objectNextPos.y) > STEP_MIN) {
			objectNextStepY = max(STEP_MIN, min(STEP_MAX, abs(objectPos.y - objectNextPos.y) / 2));
			objectPos.y += (-1) * sign(objectPos.y - objectNextPos.y) * objectNextStepY;
		}
 
	// -1 = object isn't within the camera range
	} else {
 
		objectPos.x = -1;
		objectPos.y = -1;
 
	}
 
	// Draw an object (circle) centered on the calculated center of gravity
	if (nbPixels > 10)
		cvDrawCircle(image, objectPos, 15, CV_RGB(255, 0, 0), -1);
 
	// We show the image on the window
	cvShowImage("GeckoGeek Color Tracking", image);
 
}
 
/*
 * Get the color of the pixel where the mouse has clicked
 * We put this color as model color (the color we want to tracked)
 */
void getObjectColor(int event, int x, int y, int flags, void *param = NULL) {
 
	// Vars
	CvScalar pixel;
	IplImage *hsv;
 
	if(event == CV_EVENT_LBUTTONUP)	{
 
		// Get the hsv image
		hsv = cvCloneImage(image);
		cvCvtColor(image, hsv, CV_BGR2HSV);
 
		// Get the selected pixel
		pixel = cvGet2D(hsv, y, x);
 
		// Change the value of the tracked color with the color of the selected pixel
		h = (int)pixel.val[0];
		s = (int)pixel.val[1];
		v = (int)pixel.val[2];
 
		// Release the memory of the hsv image
    		cvReleaseImage(&hsv);
 
	}
 
}
 
int main() {
 
	// Image & hsvImage
	IplImage *hsv;
	// Video Capture
	CvCapture *capture;
	// Key for keyboard event
	char key;
 
	// Number of tracked pixels
	int nbPixels;
	// Next position of the object we overlay
	CvPoint objectNextPos;
 
	// Initialize the video Capture (200 => CV_CAP_V4L2)
 	capture = cvCreateCameraCapture(200);
 
	// Check if the capture is ok
    	if (!capture) {
		printf("Can't initialize the video capture.\n");
        	return -1;
 	}
 
	// Create the windows
   	cvNamedWindow("GeckoGeek Color Tracking", CV_WINDOW_AUTOSIZE);
   	cvNamedWindow("GeckoGeek Mask", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("GeckoGeek Color Tracking", 0, 100);
	cvMoveWindow("GeckoGeek Mask", 650, 100);
 
	// Mouse event to select the tracked color on the original image
	cvSetMouseCallback("GeckoGeek Color Tracking", getObjectColor);
 
	// While we don't want to quit
	while(key != 'Q' && key != 'q') {
 
		// We get the current image
		image = cvQueryFrame(capture);
 
		// If there is no image, we exit the loop
		if(!image)
			continue;
 
		objectNextPos = binarisation(image, &nbPixels);
		addObjectToVideo(image, objectNextPos, nbPixels);
 
		// We wait 10 ms
		key = cvWaitKey(10);
 
	}
 
	// Destroy the windows we have created
	cvDestroyWindow("GeckoGeek Color Tracking");
	cvDestroyWindow("GeckoGeek Mask");
 
	// Destroy the capture
	cvReleaseCapture(&capture);
 
	return 0;
 
}
