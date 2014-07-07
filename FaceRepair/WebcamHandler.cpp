#include "WebcamHandler.h"

using namespace std;
using namespace cv;
using namespace ProcessingUtils;

WebcamHandler::WebcamHandler(int frameWidth, int frameHeight, int edgeLength, int facePositionOffset)
{
	m_loop = true;
	m_processing = false;

	m_edgeLength = edgeLength;
	m_faceAreaOffset = facePositionOffset;
	m_frameWidth = frameWidth;
	m_frameHeight = frameHeight;

	int fpHeight = max(m_edgeLength, m_frameHeight / 2);
	int fpWidth = fpHeight;
	int fpX = (m_frameWidth - fpWidth) / 2;
	int fpY = (m_frameHeight - fpHeight) / 2;
	m_faceArea = Rect(fpX, fpY, fpWidth, fpHeight);

	m_reconstructionArea = Rect(0, 0, edgeLength / 2, edgeLength);
	m_drawableReconstructionArea = scaleAndPositionReconstructionArea(&m_reconstructionArea, &m_faceArea, m_edgeLength);
}


WebcamHandler::~WebcamHandler()
{
}

void WebcamHandler::run()
{
	// initialize webcam
	VideoCapture cap = VideoCapture(0);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, m_frameWidth);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, m_frameHeight);

	// initialize windows
	namedWindow("Original", CV_WINDOW_AUTOSIZE); 
	namedWindow("Reconstruction", CV_WINDOW_AUTOSIZE);	

	while (m_loop)
	{
		// read frame and continue with next frame if not successfull
		Mat frame;
		cap.retrieve(frame);

		// flip frame
		flip(frame, frame, 1);

		// take subregion at facePosition
		Mat subregion = frame(m_faceArea);

		// copy region to seperate image
		Mat subimage;
		subregion.copyTo(subimage);

		// scale to rbm input size
		Mat scaledSubimage;
		Size size = Size(m_edgeLength, m_edgeLength);
		resize(subimage, scaledSubimage, size, 0.0, 0.0, INTER_CUBIC);

		// calc mean rgb of preserved area
		Vec3b rgb = calcRgbMeanOfPreservedArea(&scaledSubimage, &m_reconstructionArea);

		// set mean rgb at reconstructionArea
		setRgbMeanInReconstructionArea(&scaledSubimage, &m_reconstructionArea, &rgb);

		// process rbm
		// todo

		// scale to original faceArea size
		size = Size(m_faceArea.width, m_faceArea.height);
		resize(scaledSubimage, subimage, size, 0.0, 0.0, INTER_CUBIC);

		// draw rects at faceArea and reconstructionArea
		rectangle(frame, m_faceArea, Scalar(0, 255, 0), 1, 8, 0);
		rectangle(frame, m_drawableReconstructionArea, Scalar(0, 0, 255), 1, 8, 0);

		// show images
		imshow("Original", frame);
		imshow("Reconstruction", subimage);

		// check keyboard input
		checkKeys();
	}
	// terminate webcam
	cap.release();
}

void WebcamHandler::checkKeys()
{
	int key = cvWaitKey(1);
	if (key == -1) return;
	
	cout << key << endl;

	switch (key)
	{
		case 27: // escape
			m_loop = false; break; // terminate programm
		case 13: // enter
			m_processing = !m_processing; break; // turn processing on/off
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
		case 97: // a
			moveLeftReconstructionArea(); break;
		case 119: // w
			moveUpReconstructionArea(); break;
		case 100: // d
			moveRightReconstructionArea(); break;
		case 115: // s
			moveDownReconstructionArea(); break;
		case 102: // f
			decreaseWidthReconstructionArea(); break;
		case 116: // t
			increaseHeightReconstructionArea(); break;
		case 104: // h
			increaseWidthReconstructionArea(); break;
		case 103: // g
			decreaseHeightReconstructionArea(); break;
	}

	m_drawableReconstructionArea = scaleAndPositionReconstructionArea(&m_reconstructionArea, &m_faceArea, m_edgeLength);
}

void WebcamHandler::moveLeftFacePosition()
{
	m_faceArea.x = max(m_faceArea.x - m_faceAreaOffset, 0);
}

void WebcamHandler::moveUpFacePosition()
{
	m_faceArea.y = max(m_faceArea.y - m_faceAreaOffset, 0);
}

void WebcamHandler::moveRightFacePosition()
{
	m_faceArea.x = min(m_faceArea.x + m_faceAreaOffset, m_frameWidth - m_faceArea.width);
}

void WebcamHandler::moveDownFacePosition()
{
	m_faceArea.y = min(m_faceArea.y + m_faceAreaOffset, m_frameHeight - m_faceArea.height);
}

void WebcamHandler::decreaseFaceSize()
{
	m_faceArea.width = max(m_edgeLength, m_faceArea.width - m_faceAreaOffset);
	m_faceArea.height = m_faceArea.width;
}

void WebcamHandler::increaseFaceSize()
{
	int edge = m_faceArea.width + m_faceAreaOffset;
	if (m_faceArea.x + edge <= m_frameWidth && m_faceArea.y + edge <= m_frameHeight)
	{
		m_faceArea.width = edge;
		m_faceArea.height = edge;
	}
}

void WebcamHandler::moveLeftReconstructionArea(){
	m_reconstructionArea.x = max(0, m_reconstructionArea.x - 1);
}

void WebcamHandler::moveUpReconstructionArea(){
	m_reconstructionArea.y = max(0, m_reconstructionArea.y - 1);
}

void WebcamHandler::moveRightReconstructionArea(){
	m_reconstructionArea.x = min(m_reconstructionArea.x + 1, m_edgeLength - m_reconstructionArea.width);
}

void WebcamHandler::moveDownReconstructionArea(){
	m_reconstructionArea.y = min(m_reconstructionArea.y + 1, m_edgeLength - m_reconstructionArea.height);
}

void WebcamHandler::increaseHeightReconstructionArea(){
	int height = m_reconstructionArea.height + 1;
	if (m_reconstructionArea.y + height <= m_edgeLength)
	{
		m_reconstructionArea.height = height;
	}
}

void WebcamHandler::decreaseHeightReconstructionArea(){
	m_reconstructionArea.height = max(m_reconstructionArea.height - 1, 4);
}

void WebcamHandler::increaseWidthReconstructionArea(){
	int width = m_reconstructionArea.width + 1;
	if (m_reconstructionArea.x + width <= m_edgeLength)
	{
		m_reconstructionArea.width = width;
	}
}

void WebcamHandler::decreaseWidthReconstructionArea(){
	m_reconstructionArea.width = max(m_reconstructionArea.width - 1, 4);
}
