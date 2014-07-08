#include "ProcessingUtils.h"

namespace ProcessingUtils
{
	void scaleAndPositionReconstructionArea(Rect* reconstructionArea, Rect* facePosition, Rect* output, int edgeLength)
	{
		float factor = (float)facePosition->width / (float)edgeLength;
		int x = (int)(reconstructionArea->x * factor) + facePosition->x;
		int y = (int)(reconstructionArea->y * factor) + facePosition->y;
		int width = (int)(reconstructionArea->width * factor);
		int height = (int)(reconstructionArea->height * factor);

		output->x = x;
		output->y = y;
		output->width = width;
		output->height = height;
	}

	Vec3b calcRgbMeanOfPreservedArea(Mat* scaledSubimage, Rect* reconstructionArea)
	{
		double r = 0;
		double g = 0;
		double b = 0;

		int x1 = reconstructionArea->x;
		int y1 = reconstructionArea->y;
		int x2 = x1 + reconstructionArea->width;
		int y2 = y1 + reconstructionArea->height;

		int numOfPixels = (scaledSubimage->rows * scaledSubimage->cols) - (reconstructionArea->width * reconstructionArea->height);

		for (int col = 0; col < scaledSubimage->cols; ++col) {
			for (int row = 0; row < scaledSubimage->rows; ++row) {
				Vec3b rgb = scaledSubimage->at<Vec3b>(row, col);
				r += rgb[2];
				g += rgb[1];
				b += rgb[0];
			}
		}

		for (int col = x1; col < x2; ++col) {
			for (int row = y1; row < y2; ++row) {
				Vec3b rgb = scaledSubimage->at<Vec3b>(row, col);
				r -= rgb[2];
				g -= rgb[1];
				b -= rgb[0];
			}
		}

		r /= numOfPixels;
		g /= numOfPixels;
		b /= numOfPixels;

		return Vec3b((uchar)b, (uchar)g, (uchar)r);
	}

	void setRgbMeanInReconstructionArea(Mat* scaledSubimage, Rect* reconstructionArea, Vec3b* rgb)
	{
		int x1 = reconstructionArea->x;
		int y1 = reconstructionArea->y;
		int x2 = x1 + reconstructionArea->width;
		int y2 = y1 + reconstructionArea->height;

		for (int col = x1; col < x2; ++col) {
			for (int row = y1; row < y2; ++row) {
				scaledSubimage->at<Vec3b>(row, col) = *rgb;
			}
		}
	}

	float* matToNormalizedFloatArrayWithBias(Mat* image)
	{
		int rows = image->rows;
		int cols = image->cols;

		int size = cols * rows * 3 + rows;
		float* output = new float[size];

		int pos = 0;

		for (; pos < rows; ++pos){
			output[pos] = 1;
		}

		for (int col = 0; col < cols; ++col) {
			for (int row = 0; row < rows; ++row) {
				Vec3b rgb = image->at<Vec3b>(row, col);
				output[pos++] = (float)(rgb[2] / 255.0);
				output[pos++] = (float)(rgb[1] / 255.0);
				output[pos++] = (float)(rgb[0] / 255.0);
			}
		}
		return output;
	}

	void normalizedFloatArrayToMatWithoutBias(Mat* image, float* input){
		int rows = image->rows;
		int cols = image->cols;

		int pos = rows;
		cout << pos << endl;

		for (int col = 0; col < cols; ++col) {
			for (int row = 0; row < rows; ++row) {
				int r = (int)(input[pos++] * 255.0);
				int g = (int)(input[pos++] * 255.0);
				int b = (int)(input[pos++] * 255.0);
				Vec3b rgb = Vec3b(b, g, r);
				image->at<Vec3b>(row, col) = rgb;
			}
		}
	}
}
