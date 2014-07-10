#include "WebcamHandler.h"

WebcamHandler::WebcamHandler(int frameWidth, int frameHeight, int edgeLength, int threads)
{
	m_loop = true;
	m_processing = false;

	m_faceAreaOffset = 8;
	m_reconstructionAreaOffset = 2;

	m_edgeLength = edgeLength;
	m_frameWidth = frameWidth;
	m_frameHeight = frameHeight;
	
	m_webcamWidth = frameWidth / 2;
	m_webcamHeight = m_frameHeight;

	int faHeight = max(m_edgeLength, m_webcamWidth / 2);
	int faWidth = faHeight;
	int faX = (m_webcamWidth - faWidth) / 2;
	int faY = (m_webcamHeight - faHeight) / 2;

	m_faceArea = new Rect(faX, faY, faWidth, faHeight);
	m_reconstructionArea = new Rect(0, 0, edgeLength / 2, edgeLength);	
	m_drawableReconstructionArea = new Rect();
	scaleAndPositionReconstructionArea(m_reconstructionArea, m_faceArea, m_drawableReconstructionArea, m_edgeLength);
	m_scaledReconstructionArea = new Rect();
	scaleReconstructionArea(m_reconstructionArea, m_faceArea, m_scaledReconstructionArea, m_edgeLength);
	m_cropArea = new Rect((m_frameWidth - m_webcamWidth) / 2, 0, m_webcamWidth, m_webcamHeight);

	m_rbm1 = initializeRBM("C:\\Users\\christoph\\git\\FaceRepair\\FaceRepair\\weights\\WildFaces_64x64_rgb_1kh_27380it.out", threads);
}


WebcamHandler::~WebcamHandler()
{
	delete m_faceArea;
	delete m_reconstructionArea;
	delete m_drawableReconstructionArea;
	delete m_scaledReconstructionArea;
	delete m_rbm1;
}

void WebcamHandler::run()
{
	// initialize webcam
	VideoCapture cap = VideoCapture(0);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, m_frameWidth);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, m_frameHeight);

	// initialize window
	namedWindow("FaceRepair", CV_WINDOW_NORMAL);
	cvSetWindowProperty("FaceRepair", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);

	float* hidden;
	float* visible;

	while (m_loop)
	{
		// read frame and continue with next frame if not successfull
		Mat frame;
		cap.retrieve(frame);

		// mirror
		flip(frame, frame, 1);

		// crop frame for splitscreen
		Mat cropped = frame(*m_cropArea);

		// take subregion at faceArea
		Mat subregion = cropped(*m_faceArea);

		// copy region to seperate image
		Mat subimage;
		subregion.copyTo(subimage);

		// scale to rbm input size
		Mat scaledSubimage;
		Size size = Size(m_edgeLength, m_edgeLength);
		resize(subimage, scaledSubimage, size, 0.0, 0.0, INTER_CUBIC);

		// calc mean rgb of preserved area
		Vec3b rgb = calcRgbMeanOfPreservedArea(&scaledSubimage, m_reconstructionArea);

		// set mean rgb at reconstructionArea
		setRgbMeanInReconstructionArea(&scaledSubimage, m_reconstructionArea, &rgb);

		// subimage to normalized float array
		visible = matToNormalizedFloatArrayWithBias(&scaledSubimage);

		// process rbm
		int epochs = 5;

		for (int i = 0; i < epochs - 1; ++i)
		{
			hidden = m_rbm1->runHidden(visible, 1);
			delete visible;
			hidden[0] = 1;
			visible = m_rbm1->runVisible(hidden, 1);
			delete hidden;
			visible[0] = 1;
			resetPreservedArea(&scaledSubimage, m_reconstructionArea, visible);
		}

		hidden = m_rbm1->runHidden(visible, 1);
		delete visible;
		hidden[0] = 1;
		visible = m_rbm1->runVisible(hidden, 1);
		delete hidden;
		

		// normalized float array to subimage
		normalizedFloatArrayToMatWithoutBias(&scaledSubimage, visible);

		// scale to original faceArea size
		Mat result;
		size = Size(m_faceArea->width, m_faceArea->height);
		resize(scaledSubimage, result, size, 0.0, 0.0, INTER_CUBIC);

		// reset pixels of preserved area in native resolution
		resetScaledPreservedArea(&result, m_scaledReconstructionArea, &subimage);

		// draw rects at faceArea and reconstructionArea
		rectangle(cropped, *m_faceArea, Scalar(0, 255, 0), 1, 8, 0);
		rectangle(cropped, *m_drawableReconstructionArea, Scalar(0, 0, 255), 1, 8, 0);

		Mat screen = Mat(m_frameHeight, m_frameWidth, CV_8UC3, Scalar(0, 0, 0));
		makeSplitscreen(&screen, &cropped, &result);

		// show images
		imshow("FaceRepair", screen);

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
			decreaseHeightReconstructionArea(); break;
		case 104: // h
			increaseWidthReconstructionArea(); break;
		case 103: // g		
			increaseHeightReconstructionArea(); break;
	}

	scaleAndPositionReconstructionArea(m_reconstructionArea, m_faceArea, m_drawableReconstructionArea, m_edgeLength);
	scaleReconstructionArea(m_reconstructionArea, m_faceArea, m_scaledReconstructionArea, m_edgeLength);
}

void WebcamHandler::moveLeftFacePosition()
{
	m_faceArea->x = max(m_faceArea->x - m_faceAreaOffset, 0);
}

void WebcamHandler::moveUpFacePosition()
{
	m_faceArea->y = max(m_faceArea->y - m_faceAreaOffset, 0);
}

void WebcamHandler::moveRightFacePosition()
{
	m_faceArea->x = min(m_faceArea->x + m_faceAreaOffset, m_webcamWidth - m_faceArea->width);
}

void WebcamHandler::moveDownFacePosition()
{
	m_faceArea->y = min(m_faceArea->y + m_faceAreaOffset, m_webcamHeight - m_faceArea->height);
}

void WebcamHandler::decreaseFaceSize()
{
	m_faceArea->width = max(m_edgeLength, m_faceArea->width - m_faceAreaOffset);
	m_faceArea->height = m_faceArea->width;
}

void WebcamHandler::increaseFaceSize()
{
	int edge = m_faceArea->width + m_faceAreaOffset;
	if (m_faceArea->x + edge <= m_webcamWidth && m_faceArea->y + edge <= m_webcamHeight)
	{
		m_faceArea->width = edge;
		m_faceArea->height = edge;
	}
}

void WebcamHandler::moveLeftReconstructionArea(){
	m_reconstructionArea->x = max(0, m_reconstructionArea->x - m_reconstructionAreaOffset);
}

void WebcamHandler::moveUpReconstructionArea(){
	m_reconstructionArea->y = max(0, m_reconstructionArea->y - m_reconstructionAreaOffset);
}

void WebcamHandler::moveRightReconstructionArea(){
	m_reconstructionArea->x = min(m_reconstructionArea->x + m_reconstructionAreaOffset, m_edgeLength - m_reconstructionArea->width);
}

void WebcamHandler::moveDownReconstructionArea(){
	m_reconstructionArea->y = min(m_reconstructionArea->y + m_reconstructionAreaOffset, m_edgeLength - m_reconstructionArea->height);
}

void WebcamHandler::increaseHeightReconstructionArea(){
	int height = m_reconstructionArea->height + m_reconstructionAreaOffset;
	if (m_reconstructionArea->y + height <= m_edgeLength)
	{
		m_reconstructionArea->height = height;
	}
}

void WebcamHandler::decreaseHeightReconstructionArea(){
	m_reconstructionArea->height = max(m_reconstructionArea->height - m_reconstructionAreaOffset, 8);
}

void WebcamHandler::increaseWidthReconstructionArea(){
	int width = m_reconstructionArea->width + m_reconstructionAreaOffset;
	if (m_reconstructionArea->x + width <= m_edgeLength)
	{
		m_reconstructionArea->width = width;
	}
}

void WebcamHandler::decreaseWidthReconstructionArea(){
	m_reconstructionArea->width = max(m_reconstructionArea->width - m_reconstructionAreaOffset, 8);
}
