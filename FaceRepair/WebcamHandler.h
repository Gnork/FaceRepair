#pragma once
#include "opencv2\highgui\highgui.hpp"
#include <iostream>
#include <algorithm>
#include "ProcessingUtils.h"

class WebcamHandler
{
public:
	WebcamHandler(int, int, int, int);
	~WebcamHandler();
	void run();	

private:
	bool m_loop;
	bool m_processing;

	int m_frameWidth;
	int m_frameHeight;
	int m_facePositionOffset;
	int m_edgeLength;
	cv::Rect m_facePosition;
	cv::Rect m_reconstructionArea;
	cv::Rect m_drawableReconstructionArea;

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

