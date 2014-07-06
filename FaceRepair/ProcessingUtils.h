#include "opencv2\highgui\highgui.hpp"
#include "opencv2\opencv.hpp"
#include "opencv2\imgproc\imgproc.hpp"

using namespace cv;

namespace ProcessingUtils{
	Rect scaleAndPositionReconstructionArea(Rect* reconstructionArea, Rect* facePosition, int edgeLength);
	Scalar calcRgbMeanOfPreservedArea(Mat* scaledSubimage, Rect* reconstructionArea);
}