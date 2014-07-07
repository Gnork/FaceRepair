#pragma once
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\opencv.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include <iostream>

using namespace cv;

namespace ProcessingUtils{
	void scaleAndPositionReconstructionArea(Rect* reconstructionArea, Rect* facePosition, Rect* output, int edgeLength);
	Vec3b calcRgbMeanOfPreservedArea(Mat* scaledSubimage, Rect* reconstructionArea);
	void setRgbMeanInReconstructionArea(Mat* scaledSubimage, Rect* reconstructionArea, Vec3b* rgb);
}