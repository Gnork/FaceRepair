#include "ProcessingUtils.h"

using namespace cv;

namespace ProcessingUtils{
	Rect scaleAndPositionReconstructionArea(Rect* reconstructionArea, Rect* facePosition, int edgeLength)
	{
		float factor = (float)facePosition->width / (float)edgeLength;
		int x = (int)(reconstructionArea->x * factor) + facePosition->x;
		int y = (int)(reconstructionArea->y * factor) + facePosition->y;
		int width = (int)(reconstructionArea->width * factor);
		int height = (int)(reconstructionArea->height * factor);

		return Rect(x, y, width, height);
	}

	Scalar calcRgbMeanOfPreservedArea(Mat* scaledSubimage, Rect* reconstructionArea)
	{
		double r = 0;
		double g = 0;
		double b = 0;

		int x1 = reconstructionArea->x;
		int y1 = reconstructionArea->y;
		int x2 = x1 + reconstructionArea->width;
		int y2 = y1 + reconstructionArea->height;

		int numOfPixels = (scaledSubimage->rows * scaledSubimage->cols) - (reconstructionArea->width * reconstructionArea->height);

		for (int row = 0; row < scaledSubimage->rows; ++row) {
			if (row >= x1 && row <= x2) continue;
			for (int col = 0; col < scaledSubimage->cols; ++col) {
				if (col >= y1 && col <= y2) continue;
				Vec3b rgb = scaledSubimage->at<Vec3b>(row, col);
				r += rgb[2];
				g += rgb[1];
				b += rgb[0];
			}
		}

		r /= numOfPixels;
		g /= numOfPixels;
		b /= numOfPixels;

		return Scalar(b, g, r);
	}
}
