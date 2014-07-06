#include "WebcamHandler.h"

using namespace std;
using namespace cv;

WebcamHandler::WebcamHandler()
{
	m_facePosition = Rect(300, 300, 80, 80);
	m_loop = true;
}


WebcamHandler::~WebcamHandler()
{
}

void WebcamHandler::run()
{
	
	VideoCapture cap(0);	

	if (!cap.isOpened())
	{
		cout << "Cannot open the video file" << endl;
		m_loop = false;
	}

	namedWindow("Original", CV_WINDOW_AUTOSIZE); 
	namedWindow("Reconstruction", CV_WINDOW_AUTOSIZE);	

	while (m_loop)
	{
		// read frame and continue with next frame if not successfull
		Mat frame;
		bool success = cap.read(frame);
		if (!success) {continue;}

		// flip frame
		flip(frame, frame, 1);

		// take subregion at facePosition
		Mat subregion = frame(m_facePosition);

		// copy region to seperate image
		Mat subimage;
		subregion.copyTo(subimage);

		// draw rect at facePosition
		rectangle(frame, m_facePosition, Scalar(0, 255, 0), 1, 8, 0);

		//show images
		imshow("Original", frame);
		imshow("Reconstruction", subimage);

		// check keyboard input
		checkKeys();
	}
	cap.release();
}

void WebcamHandler::checkKeys()
{
	if (waitKey(30) == 27)
	{
		cout << "esc key is pressed by user" << endl;
		m_loop = false;
	}
}
