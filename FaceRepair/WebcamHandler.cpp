#include "WebcamHandler.h"

using namespace std;
using namespace cv;

WebcamHandler::WebcamHandler(int frameWidth, int frameHeight, int edgeLength, int facePositionOffset)
{
	m_loop = true;
	m_edgeLength = edgeLength;

	m_facePositionOffset = facePositionOffset;

	m_frameWidth = frameWidth;
	m_frameHeight = frameHeight;

	int fpHeight = max(m_edgeLength, m_frameHeight / 2);
	int fpWidth = fpHeight;
	int fpX = (m_frameWidth - fpWidth) / 2;
	int fpY = (m_frameHeight - fpHeight) / 2;
	m_facePosition = Rect(fpX, fpY, fpWidth, fpHeight);
}


WebcamHandler::~WebcamHandler()
{
}

void WebcamHandler::run()
{
	// initialize webcam
	CvCapture* capture = cvCreateCameraCapture(0);
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, m_frameWidth);
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, m_frameHeight);

	// initialize windows
	namedWindow("Original", CV_WINDOW_AUTOSIZE); 
	namedWindow("Reconstruction", CV_WINDOW_AUTOSIZE);	

	while (m_loop)
	{
		// read frame and continue with next frame if not successfull
		Mat frame;
		frame = cvRetrieveFrame(capture, 0);

		// flip frame
		flip(frame, frame, 1);

		// take subregion at facePosition
		Mat subregion = frame(m_facePosition);

		// copy region to seperate image
		Mat subimage;
		subregion.copyTo(subimage);

		// draw rect at facePosition
		rectangle(frame, m_facePosition, Scalar(0, 255, 0), 1, 8, 0);

		// show images
		imshow("Original", frame);
		imshow("Reconstruction", subimage);

		// check keyboard input
		checkKeys();
	}
	// terminate webcam
	cvReleaseCapture(&capture);
}

void WebcamHandler::checkKeys()
{
	int key = cvWaitKey(1);
	if(key != -1) cout << key << endl;

	switch (key)
	{
		case 27: // escape
			m_loop = false; break; // terminate programm
		case 2424832: // left arrow
			moveLeftFacePosition(); break;
		case 2490368: // up arrow
			moveUpFacePosition(); break;
		case 2555904: // right arrow
			moveRightFacePosition(); break;
		case 2621440: // down arrow
			moveDownFacePosition(); break;
		case 110: // n
			decreaseFaceSize(); break;
		case 109: // m
			increaseFaceSize(); break;
	}
}

void WebcamHandler::moveLeftFacePosition()
{
	m_facePosition.x = max(m_facePosition.x - m_facePositionOffset, 0);
}

void WebcamHandler::moveUpFacePosition()
{
	m_facePosition.y = max(m_facePosition.y - m_facePositionOffset, 0);
}

void WebcamHandler::moveRightFacePosition()
{
	m_facePosition.x = min(m_facePosition.x + m_facePositionOffset, m_frameWidth - m_facePosition.width);
}

void WebcamHandler::moveDownFacePosition()
{
	m_facePosition.y = min(m_facePosition.y + m_facePositionOffset, m_frameHeight - m_facePosition.height);
}

void WebcamHandler::decreaseFaceSize()
{
	m_facePosition.width = max(m_edgeLength, m_facePosition.width - m_facePositionOffset);
	m_facePosition.height = m_facePosition.width;
}

void WebcamHandler::increaseFaceSize()
{
	int edge = m_facePosition.width + m_facePositionOffset;
	if (m_facePosition.x + edge <= m_frameWidth && m_facePosition.y + edge <= m_frameHeight)
	{
		m_facePosition.width = edge;
		m_facePosition.height = edge;
	}
}
