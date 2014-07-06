#include "opencv2\highgui\highgui.hpp"

using namespace cv;

namespace ProcessingUtils{
	Rect scaleAndPositionReconstructionArea(Rect* reconstructionArea, Rect* facePosition, int edgeLength);
}