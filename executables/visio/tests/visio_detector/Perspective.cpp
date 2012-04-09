#include "Perspective.h"

using namespace cv;

void Perspective::calculateTransformation()
{
	// proportions de l'échiquier en pixels
	double chess_lengthX_01 = sqrt(
	        pow(src_vertices[1].x - src_vertices[0].x, 2) +
	        pow(src_vertices[1].y - src_vertices[0].y, 2) );
	double chess_lengthY_02 = chess_lengthX_01 / cellsX * cellsY;

	double xmin = 1;
	double ymin = 1;
	src_vertices[0] = Point2f(src_vertices[0].x + xmin, src_vertices[0].y + ymin);
	src_vertices[1] = Point2f(src_vertices[1].x + xmin, src_vertices[1].y + ymin);
	src_vertices[2] = Point2f(src_vertices[2].x + xmin, src_vertices[2].y + ymin);
	src_vertices[3] = Point2f(src_vertices[3].x + xmin, src_vertices[3].y + ymin);

	// plan caméra
	int deplace = 200;
	dst_vertices[0] = Point2f(src_vertices[0].x + deplace, src_vertices[0].y + deplace);
	dst_vertices[1] = Point2f(src_vertices[1].x + deplace, src_vertices[0].y + deplace);
	dst_vertices[2] = Point2f(src_vertices[0].x + deplace, src_vertices[0].y - chess_lengthY_02 + deplace);
	dst_vertices[3] = Point2f(src_vertices[1].x + deplace, src_vertices[0].y - chess_lengthY_02 + deplace);

	// dst_vertices[0] = Point2f(src_vertices[0].x, src_vertices[0].y);
	// dst_vertices[1] = Point2f(src_vertices[0].x - chess_lengthX_01, src_vertices[0].y);
	// dst_vertices[2] = Point2f(src_vertices[0].x, src_vertices[0].y - chess_lengthY_02);
	// dst_vertices[3] = Point2f(src_vertices[0].x - chess_lengthX_01, src_vertices[0].y - chess_lengthY_02);

	chess_x_mmBypixels = cellXmm / (chess_lengthX_01 / cellsX);
	chess_y_mmBypixels = cellYmm / (chess_lengthY_02 / cellsY);

	// on calcule la perspective avec le plan trouvé
	chessWarpMatrix = getPerspectiveTransform(src_vertices, dst_vertices);
};

// cellsX >= 2 && cellsY >= 2
Perspective::Perspective(int cellsX, int cellsY, float cellXmm, float cellYmm) {
	this->cellsX = cellsX;
	this->cellsY = cellsY;
	this->chess_board_size = Size(cellsX, cellsY);
	this->cellXmm = cellXmm;
	this->cellYmm = cellYmm;

	// par défaut on fait aucune transformation
	reset();
};

// Trouve l'échiquier dans l'image n&b ou couleur
bool Perspective::findChessboard(Mat &img)
{
    chess_found = findChessboardCorners(img, chess_board_size, chess_cells_points_found);
    if (chess_found) {
        src_vertices[0] = chess_cells_points_found[0];
        src_vertices[1] = chess_cells_points_found[4];
        src_vertices[2] = chess_cells_points_found[15];
        src_vertices[3] = chess_cells_points_found[19];

        calculateTransformation();
    }

    // drawChessboardCorners: affiche l'échiquier trouvé
    // drawChessboardCorners(pyr, chess_board_size, Mat(chess_cells_points_found), chess_found);

    return chess_found;
};

bool Perspective::ready()
{
	return chess_found; // ready do to perspective
};

void Perspective::reset()
{
	chess_found = false;

	// ici on initialise avec les mêmes plans
	// donc le changement de perspective de fait rien
	this->src_vertices[0] = Point(0, 0);
	this->src_vertices[1] = Point(10, 0);
	this->src_vertices[2] = Point(10, 10);
	this->src_vertices[3] = Point(0, 10);

	this->dst_vertices[0] = Point(0, 0);
	this->dst_vertices[1] = Point(10, 0);
	this->dst_vertices[2] = Point(10, 10);
	this->dst_vertices[3] = Point(0, 10);

	// on initialise la matrix pour éviter un pointeur null
	this->chessWarpMatrix = getPerspectiveTransform(src_vertices, dst_vertices);
};

// Affiche les 4 bords de l'échiquier sur l'image donnée
void Perspective::drawBorders(Mat &img)
{
    circle(img, src_vertices[0], 9, Scalar(255, 0, 0), -1, 8, 0 );
    circle(img, src_vertices[1], 9, Scalar(0, 255, 0), -1, 8, 0 );
    circle(img, src_vertices[2], 9, Scalar(0, 255, 0), -1, 8, 0 );
    //circle(img, src_vertices[3], 9, Scalar(0, 0, 0), -1, 8, 0 );

    putText(img, "0", src_vertices[0], FONT_HERSHEY_SIMPLEX, 0.3, Scalar(255, 255, 255));
    putText(img, "1 X", src_vertices[1], FONT_HERSHEY_SIMPLEX, 0.3, Scalar(255, 255, 255));
    putText(img, "2 Y", src_vertices[2], FONT_HERSHEY_SIMPLEX, 0.3, Scalar(255, 255, 255));
    //putText(img, "3", src_vertices[3], FONT_HERSHEY_SIMPLEX, 0.3, Scalar(255, 255, 255));
};

// Redresse le plan de l'image donnée à celui de la caméra
void Perspective::wrapPerspective(Mat &in, Mat &out)
{
	int multi = 3;
	CvPoint p;
	p.x = -3;
	p.y = -1;
    warpPerspective(in, out, chessWarpMatrix, Size(in.cols*multi, in.rows*multi), INTER_LINEAR, BORDER_CONSTANT);
    // BORDER_CONSTANT => bordures noires
    // BORDER_WRAP => mozaik
    // BORDER_REFLECT_101 => mozaik mirroire
    // BORDER_TRANSPARENT => touche pas au fond
};

// Calcule la position du pixel donné de l'image redressée par rapport
// à l'origine de l'échiquier, le tout en milimètre.
CvPoint2D32f Perspective::realDistanceToChessOrigin(Point imagePoint)
{
	float mmX = abs(imagePoint.x - dst_vertices[0].x) * chess_x_mmBypixels;
	float mmY = abs(imagePoint.y - dst_vertices[0].y) * chess_y_mmBypixels;

	CvPoint2D32f realPoint;
	realPoint.x = mmX;
	realPoint.y = mmY;

	return realPoint;
};
