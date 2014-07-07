#include "opencv2\highgui\highgui.hpp"
#include "opencv2\opencv.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include <iostream>

using namespace cv;

namespace ProcessingUtils{
	Rect scaleAndPositionReconstructionArea(Rect* reconstructionArea, Rect* facePosition, int edgeLength);
	Vec3b calcRgbMeanOfPreservedArea(Mat* scaledSubimage, Rect* reconstructionArea);
	void setRgbMeanInReconstructionArea(Mat* scaledSubimage, Rect* reconstructionArea, Vec3b* rgb);
}