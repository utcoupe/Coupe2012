#ifndef DEF_Perspective
#define DEF_Perspective

#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

class Perspective {
	Size chess_board_size; // nombres de cases - 1
	int cellsX;
	int cellsY;
	float cellXmm, cellYmm; // vraies dimenssions d'une case

	Point2f src_vertices[4]; // src - plan échiquier
	Point2f dst_vertices[4]; // dst - plan redressé
	float chess_x_mmBypixels; // échelle - pixels <-> milimètres
	float chess_y_mmBypixels;

	vector<Point2f> chess_cells_points_found; // toutes les cellules de l'échiquier
	bool chess_found; // si on a trouvé l'échiquier
	Mat chessWarpMatrix; // matrice de transformation dst = M(src)

	// On calcule la plan de la caméra par rapport au plan de l'échiquier trouvé
	void calculateTransformation();

public:
	// cellsX >= 2 && cellsY >= 2
    Perspective(int cellsX, int cellsY, float cellXmm, float cellYmm);

    // Trouve l'échiquier dans l'image n&b ou couleur
    bool findChessboard(Mat &img);

    bool ready();

    void reset();

    // Affiche les 4 bords de l'échiquier sur l'image donnée
    void drawBorders(Mat &img);

    // Redresse le plan de l'image donnée à celui de la caméra
    void wrapPerspective(Mat &in, Mat &out);
    
    // Calcule la position du pixel donné de l'image redressée par rapport
	// à l'origine de l'échiquier, le tout en milimètre.
    CvPoint2D32f realDistanceToChessOrigin(Point imagePoint);
};

#endif
