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
	WebcamHandler(int frameWidth, int frameHeight, int edgeLength, int facePositionOffset, int threads);
	~WebcamHandler();
	void run();	

private:
	bool m_loop;
	bool m_processing;

	int m_frameWidth;
	int m_frameHeight;
	int m_faceAreaOffset;
	int m_edgeLength;
	Rect* m_faceArea;
	Rect* m_reconstructionArea;
	Rect* m_drawableReconstructionArea;

	RBM* m_rbm1;

	void checkKeys();

	void moveLeftFacePosition();
	void moveUpFacePosition();
	void moveRightFacePosition();
	void moveDownFacePosition();
	void decreaseFaceSize();
	void increaseFaceSize();

	void moveLeftReconstructionArea();
	void moveUpReconstructionArea();
	void moveRightReconstructionArea();
	void moveDownReconstructionArea();
	void increaseHeightReconstructionArea();
	void decreaseHeightReconstructionArea();
	void increaseWidthReconstructionArea();
	void decreaseWidthReconstructionArea();
};

