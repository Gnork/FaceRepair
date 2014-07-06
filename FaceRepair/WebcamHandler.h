#pragma once
#include "opencv2\highgui\highgui.hpp"
#include <iostream>

class WebcamHandler
{
public:
	WebcamHandler();
	~WebcamHandler();
	void run();

private:
	bool m_loop;
	cv::Rect m_facePosition;
	void checkKeys();
};

