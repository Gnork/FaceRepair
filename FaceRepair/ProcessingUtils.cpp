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
}
