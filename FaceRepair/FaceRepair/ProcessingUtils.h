#pragma once
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\opencv.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

namespace ProcessingUtils
{
	float* matToNormalizedFloatArrayWithBias(Mat*);
	void normalizedFloatArrayToMatWithoutBias(float*, Mat*);
	void resetPreservedArea(Mat*, Mat*, float*);
	void printDetectionColor(Mat* src);
}