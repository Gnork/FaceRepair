#pragma once
#include "opencv2\highgui\highgui.hpp"
#include <iostream>
#include <algorithm>
#include "ProcessingUtils.h"
#include "RBMUtils.h"
#include "opencv2\opencv.hpp"
#include "RBM.h"

using namespace std;
using namespace cv;
using namespace ProcessingUtils;
using namespace RBMUtils;

class WebcamHandler
{
public:
	WebcamHandler(int, int, int, int);
	~WebcamHandler();
	void run();	

private:
	bool m_loop;

	int m_frameWidth;
	int m_frameHeight;
	int m_faceAreaOffset;
	int m_edgeLength;
	Rect* m_faceArea;

	Scalar* m_detectionColorMin;
	Scalar* m_detectionColorMax;

	RBM* m_rbm1;
	RBM* m_rbm2;

	void checkKeys();

	void moveLeftFacePosition();
	void moveUpFacePosition();
	void moveRightFacePosition();
	void moveDownFacePosition();
	void decreaseFaceSize();
	void increaseFaceSize();
};

