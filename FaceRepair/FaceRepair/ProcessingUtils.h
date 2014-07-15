#pragma once
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\opencv.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

namespace ProcessingUtils
{
	Mat fullscreen(Mat* src1, Mat* src2, Mat* src3, int pixelSpacer);
	float* matToNormalizedFloatArrayWithBias(Mat* src);
	void normalizedFloatArrayToMatWithoutBias(float* src, Mat* dst);
	void resetPreservedArea(Mat* image, Mat* mask, float* visible);
	void printDetectionColor(Mat* src);
}