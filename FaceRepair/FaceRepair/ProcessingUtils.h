#pragma once
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\opencv.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

namespace ProcessingUtils
{
	void scaleAndPositionReconstructionArea(Rect*, Rect*, Rect*, int);
	Vec3b calcRgbMeanOfPreservedArea(Mat*, Rect*);
	void setRgbMeanInReconstructionArea(Mat*, Rect*, Vec3b*);
	float* matToNormalizedFloatArrayWithBias(Mat*);
	void normalizedFloatArrayToMatWithoutBias(Mat*, float*);
	void resetPreservedArea(Mat*, Rect*, float*);
	void scaleReconstructionArea(Rect*, Rect*, Rect*, int);
	void resetScaledPreservedArea(Mat*, Rect*, Mat*);
	void makeSplitscreen(Mat*, Mat*, Mat*);
}