#pragma once
#include "opencv2\highgui\highgui.hpp"
#include <iostream>
#include <algorithm>
#include "ProcessingUtils.h"
#include "RBMUtils.h"
#include "opencv2\opencv.hpp"
#include "RBM.h"
#include <chrono>
#include <thread>

using namespace std;
using namespace cv;
using namespace ProcessingUtils;
using namespace RBMUtils;

class WebcamHandler
{
public:
	WebcamHandler();
	~WebcamHandler();
	void run();	

private:
	bool m_loop;
	bool m_action;

	int m_frameWidth;
	int m_frameHeight;
	int m_faceAreaOffset;
	int m_edgeLength;
	int m_threads;
	Rect* m_faceArea;
	Rect* m_classifierArea;

	Scalar* m_detectionColorMin;
	Scalar* m_detectionColorMax;

	RBM* m_rbm1000;
	RBM* m_rbm1500;
	RBM* m_rbm2000;

	void checkKeys();

	void moveLeftFacePosition();
	void moveUpFacePosition();
	void moveRightFacePosition();
	void moveDownFacePosition();
	void decreaseFaceSize();
	void increaseFaceSize();
};

