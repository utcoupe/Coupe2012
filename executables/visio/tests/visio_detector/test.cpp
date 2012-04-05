#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include <stdio.h>

#include "FPS.h"
#include "Perspective.h"
#include "ColorPicker.h"
#include "ColorTracker.h"
#include "CircleTracker.h"

using namespace cv;
using namespace std;


void help(char** av)
{
    cout << "\nCe programme prend un argument.\n"
        "Une image, une vidéo ou une caméra (=0).\n"
        << "\tespace: sauver image\n"
        << "\tq,Q,échap: quitter\n"
        << "\tc: détecter chessboard\n"
        << endl;
}

int process(VideoCapture& capture, /* OR */ Mat& image)
{
    // ------------------------------------
    // Déclarations
    // ------------------------------------

    Mat frame; // image source qui change à chaque boucle (ne pas modifier durant la boucle)

    FPS fpsshower;
    Perspective pers(5, 4, 28.5, 28.5);

    // options contrôlables avec le clavier
    bool showChessBorders = false; // 4 bords de l'échiquier
    bool wrapPerspective = true; // activer la perspective
    bool findChessboard = false; // active la recherche de l'échiquier
    bool pauseCapture = false; // pauseCapture la vidéo
    int time_keys = 30; // ~13 FPS avec visio comprise


    namedWindow( "One", 1);
    namedWindow( "Two", 1);

    colorTrackerInit();
    //circleTrackerInit();

    // ------------------------------------
    // Boucle !
    // ------------------------------------

    for (;;)
    {

        // ------------------------------------
        // Capture de l'image
        // ------------------------------------
        
        if (!image.empty()) frame = image.clone();
        else if (!pauseCapture)
        {
            capture >> frame;
            if (frame.empty()) continue;
        }

        // ------------------------------------
        // Perspective
        // ------------------------------------

        Mat pyr, gray;
        Mat rotated;
        //pyrDown(frame, pyr, Size(frame.cols/2, frame.rows/2)); // reduire
        pyr = frame;
        cvtColor(pyr, gray, CV_RGB2GRAY);

        if (findChessboard)
        {
            bool found = pers.findChessboard(gray);
            if (found) puts("Chessboard found.");
            findChessboard = false; // one time
        }

        if (showChessBorders && pers.ready())
        {
            pers.drawBorders(pyr);
        }

        if (wrapPerspective)
        {
            pers.wrapPerspective(pyr, rotated);
        }
        else
        {
            rotated = pyr;
        }

        // ------------------------------------
        // Circles (puissant avec Canny et Gaussian)
        // ------------------------------------

        //circleTracker(rotated);

        // ------------------------------------
        // Color tracker
        // ------------------------------------

        colorTracker(rotated, pers);

        // ------------------------------------
        // Show FPS
        // ------------------------------------

        fpsshower.tic();
        fpsshower.draw(pyr);

        // ------------------------------------
        // Draw
        // ------------------------------------

        imshow("One", pyr);
        imshow("Two", rotated);
        // --------------------------

        char key = (char) waitKey(time_keys); //delay N millis, usually long enough to display and capture input
        switch (key)
        {
            case 'c':
                findChessboard = !findChessboard;
                break;
            case 'i':
                pers.reset();
                break;
            case 'b':
                showChessBorders = !showChessBorders;
                break;
            case 'e':
                wrapPerspective = !wrapPerspective;
                break;

            case 'r':
                ColorPickerResetAverage();
                break;

            case 'a':
                time_keys++;
                printf("Keys time = %d\n", time_keys);
                break;
            case 'z':
                time_keys--;
                printf("Keys time = %d\n", time_keys);
                break;

            case 'p':
                pauseCapture = !pauseCapture;
                break;

            case ' ': //Save an image
                puts("save...");
                char filename[200];
                sprintf(filename, "filename%.5d.jpg", rand());
                imwrite(filename, pyr);
                char filename2[200];
                sprintf(filename2, "filename%.5d.jpg", rand());
                imwrite(filename2, rotated);
                puts("done.");
                break;

            case 'q':
            case 'Q':
            case 27: //escape key
                return 0;
            default:
                break;
        }
    }
    return 0;
}


int main(int ac, char** av)
{
    // On a besoin de UN argument
    if (ac != 2)
    {
        help(av);
        return 1;
    }
    std::string arg = av[1];

    // Try loading image
    Mat image = imread(arg, 1);
    if (!image.empty())
    {
        resize(image, image, Size(), 0.3, 0.3, INTER_LINEAR);
    }

    // Try video input (or camera)
    VideoCapture capture(arg); //try to open string, this will attempt to open it as a video file
    if (!capture.isOpened()) //if this fails, try to open as a video camera, through the use of an integer param
        capture.open(atoi(arg.c_str()));
    if (!capture.isOpened())
    {
        cerr << "Failed to open a video device or video file!\n" << endl;
        help(av);
        return 1;
    }

    return process(capture, image);
}
