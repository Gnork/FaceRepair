#include "ProcessingUtils.h"

namespace ProcessingUtils
{
	void scaleAndPositionReconstructionArea(Rect* reconstructionArea, Rect* faceArea, Rect* output, int edgeLength)
	{
		float factor = (float)faceArea->width / (float)edgeLength;
		int x = (int)(reconstructionArea->x * factor) + faceArea->x;
		int y = (int)(reconstructionArea->y * factor) + faceArea->y;
		int width = (int)(reconstructionArea->width * factor);
		int height = (int)(reconstructionArea->height * factor);

		output->x = x;
		output->y = y;
		output->width = width;
		output->height = height;
	}

	void scaleReconstructionArea(Rect* reconstructionArea, Rect* faceArea, Rect* output, int edgeLength)
	{
		float factor = (float)faceArea->width / (float)edgeLength;
		int x = (int)(reconstructionArea->x * factor);
		int y = (int)(reconstructionArea->y * factor);
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

		int size = cols * rows * 3 + 1;
		float* output = new float[size];

		int pos = 0;
		output[pos++] = 1;

		for (int row = 0; row < rows; ++row) {
			for (int col = 0; col < cols; ++col) {						
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

		int pos = 1;

		
		for (int row = 0; row < rows; ++row) {
			for (int col = 0; col < cols; ++col) {		
				int r = (int)(input[pos++] * 255.0);
				int g = (int)(input[pos++] * 255.0);
				int b = (int)(input[pos++] * 255.0);
				Vec3b rgb = Vec3b(b, g, r);
				image->at<Vec3b>(row, col) = rgb;
			}
		}
	}

	void resetPreservedArea(Mat* scaledSubimage, Rect* reconstructionArea, float* visible)
	{
		int width = reconstructionArea->width;
		int height = reconstructionArea->height;

		int x1 = reconstructionArea->x;
		int y1 = reconstructionArea->y;
		int x2 = x1 + width;
		int y2 = y1 + height;

		int cols = scaledSubimage->cols;
		int rows = scaledSubimage->rows;

		for (int row = 0; row < rows; ++row)
		{
			for (int col = 0; col < x1; ++col)
			{
				Vec3b rgb = scaledSubimage->at<Vec3b>(row, col);
				int pos = (cols * row + col) * 3 + 1;
				visible[pos] = rgb[2] / 255.0f;
				visible[pos + 1] = rgb[1] / 255.0f;
				visible[pos + 2] = rgb[0] / 255.0f;
			}
			for (int col = x2; col < cols; ++col)
			{
				Vec3b rgb = scaledSubimage->at<Vec3b>(row, col);
				int pos = (cols * row + col) * 3 + 1;
				visible[pos] = rgb[2] / 255.0f;
				visible[pos + 1] = rgb[1] / 255.0f;
				visible[pos + 2] = rgb[0] / 255.0f;
			}
		}

		for (int col = 0; col < cols; ++col)
		{
			for (int row = 0; row < y1; ++row)
			{
				Vec3b rgb = scaledSubimage->at<Vec3b>(row, col);
				int pos = (cols * row + col) * 3 + 1;
				visible[pos] = rgb[2] / 255.0f;
				visible[pos + 1] = rgb[1] / 255.0f;
				visible[pos + 2] = rgb[0] / 255.0f;
			}
			for (int row = y2; row < rows; ++row)
			{
				Vec3b rgb = scaledSubimage->at<Vec3b>(row, col);
				int pos = (cols * row + col) * 3 + 1;
				visible[pos] = rgb[2] / 255.0f;
				visible[pos + 1] = rgb[1] / 255.0f;
				visible[pos + 2] = rgb[0] / 255.0f;
			}
		}

		/*
		for (int row = 0; row < rows; ++row) 
		{
			for (int col = 0; col < cols; ++col) 
			{	
				if (row >= y1 && row < y2 && col >= x1 && col < x2) continue;
				Vec3b rgb = scaledSubimage->at<Vec3b>(row, col);
				int pos = (cols * row + col) * 3 + 1;
				visible[pos] = rgb[2] / 255.0f;
				visible[pos + 1] = rgb[1] / 255.0f;
				visible[pos + 2] = rgb[0] / 255.0f;
			}
		}
		*/
	}

	void resetScaledPreservedArea(Mat* dst, Rect* area, Mat* src)
	{
		int width = area->width;
		int height = area->height;

		int x1 = area->x;
		int y1 = area->y;
		int x2 = x1 + width;
		int y2 = y1 + height;

		int cols = dst->cols;
		int rows = dst->rows;

		for (int row = 0; row < rows; ++row)
		{
			for (int col = 0; col < x1; ++col)
			{
				dst->at<Vec3b>(row, col) = src->at<Vec3b>(row, col);
			}
			for (int col = x2; col < cols; ++col)
			{
				dst->at<Vec3b>(row, col) = src->at<Vec3b>(row, col);
			}
		}

		for (int col = 0; col < cols; ++col)
		{
			for (int row = 0; row < y1; ++row)
			{
				dst->at<Vec3b>(row, col) = src->at<Vec3b>(row, col);
			}
			for (int row = y2; row < rows; ++row)
			{
				dst->at<Vec3b>(row, col) = src->at<Vec3b>(row, col);
			}
		}
	}

	void makeSplitscreen(Mat* screen, Mat* frame, Mat* reconstruction)
	{
		int screenCols = screen->cols;
		int screenRows = screen->rows;
		int frameCols = frame->cols;
		int frameRows = frame->rows;
		int reconstructionCols = reconstruction->cols;
		int reconstructionRows = reconstruction->rows;

		for (int row = 0; row < frameRows; ++row)
		{
			for (int col = 0; col < frameCols; ++col)
			{
				screen->at<Vec3b>(row, col) = frame->at<Vec3b>(row, col);
			}
		}
	}
}
