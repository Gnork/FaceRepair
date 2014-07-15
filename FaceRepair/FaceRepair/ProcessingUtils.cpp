#include "ProcessingUtils.h"

namespace ProcessingUtils
{
	Mat fullscreen(Mat* src1, Mat* src2, Mat* src3, int pixelSpacer)
	{
		int edgeLength = src1->cols;
		int totalWidth = 3 * edgeLength + 2 * pixelSpacer;
		int totalHeight = (int)(totalWidth / 16.0 * 9.0);
		int yOffset = (totalHeight - edgeLength) / 2;

		Mat screen = Mat(totalHeight, totalWidth, CV_8UC3, Scalar(0, 0, 0));
		src1->copyTo(screen(Rect(0, yOffset, edgeLength, edgeLength)));
		src2->copyTo(screen(Rect(edgeLength + pixelSpacer, yOffset, edgeLength, edgeLength)));
		src3->copyTo(screen(Rect(edgeLength * 2 + pixelSpacer * 2, yOffset, edgeLength, edgeLength)));

		return screen;
	}

	void printDetectionColor(Mat* src)
	{
		int size = 64;

		int rows = src->rows;
		int cols = src->cols;

		int h = 0;
		int s = 0;
		int v = 0;

		int minH = 255;
		int minS = 255;
		int minV = 255;

		int maxH = 0;
		int maxS = 0;
		int maxV = 0;

		int len = size * size;

		for (int row = 0; row < size; ++row)
		{
			for (int col = 0; col < size; ++col)
			{
				Vec3b hsv = src->at<Vec3b>(row, col);
				h += hsv[0];
				s += hsv[1];
				v += hsv[2];

				minH = min(minH, (int)hsv[0]);
				minS = min(minS, (int)hsv[1]);
				minV = min(minV, (int)hsv[2]);

				maxH = max(maxH, (int)hsv[0]);
				maxS = max(maxS, (int)hsv[1]);
				maxV = max(minV, (int)hsv[2]);
			}
		}

		h /= len;
		s /= len;
		v /= len;

		cout << " min: " << minH << ", " << minS << ", " << minV << endl;
		cout << "mean: " << h << ", " << s << ", " << v << endl;
		cout << " max: " << maxH << ", " << maxS << ", " << maxV << endl;	
	}

	float* matToNormalizedFloatArrayWithBias(Mat* src)
	{
		int rows = src->rows;
		int cols = src->cols;

		int size = cols * rows * 3 + 1;
		float* output = new float[size];

		int pos = 0;
		output[pos++] = 1;

		for (int row = 0; row < rows; ++row) {
			for (int col = 0; col < cols; ++col) {						
				Vec3b rgb = src->at<Vec3b>(row, col);
				output[pos++] = (float)(rgb[2] / 255.0);
				output[pos++] = (float)(rgb[1] / 255.0);
				output[pos++] = (float)(rgb[0] / 255.0);
			}
		}
		return output;
	}

	void normalizedFloatArrayToMatWithoutBias(float* src, Mat* dst){
		int rows = dst->rows;
		int cols = dst->cols;

		int pos = 1;
	
		for (int row = 0; row < rows; ++row) {
			for (int col = 0; col < cols; ++col) {		
				int r = (int)(src[pos++] * 255.0);
				int g = (int)(src[pos++] * 255.0);
				int b = (int)(src[pos++] * 255.0);
				Vec3b rgb = Vec3b(b, g, r);
				dst->at<Vec3b>(row, col) = rgb;
			}
		}
	}

	void resetPreservedArea(Mat* image, Mat* mask, float* visible)
	{
		int rows = image->rows;
		int cols = image->cols;

		for (int row = 0; row < rows; ++row) 
		{
			for (int col = 0; col < cols; ++col) 
			{
				if(mask->at<uchar>(row, col) == 0) continue;
				Vec3b rgb = image->at<Vec3b>(row, col);
				int pos = (cols * row + col) * 3 + 1;
				visible[pos] = rgb[2] / 255.0f;
				visible[pos + 1] = rgb[1] / 255.0f;
				visible[pos + 2] = rgb[0] / 255.0f;
			}
		}
	}
}
