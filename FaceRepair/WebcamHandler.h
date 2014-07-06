#pragma once
#include "opencv2\highgui\highgui.hpp"
#include <iostream>
#include <algorithm>

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
	int m_facePositionOffset;
	int m_edgeLength;
	cv::Rect m_facePosition;

	void checkKeys();
	void moveLeftFacePosition();
	void moveUpFacePosition();
	void moveRightFacePosition();
	void moveDownFacePosition();
	void decreaseFaceSize();
	void increaseFaceSize();
};

