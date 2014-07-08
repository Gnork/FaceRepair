
#ifndef RBM_H_
#define RBM_H_

#include <cblas.h>
#include <cmath>
#include <algorithm>
#include <omp.h>
#include <chrono>

#include <iostream>
#include <iomanip>

class RBM
{
public:
	RBM(float* weights, int weightRows, int weightsCols, int threads) :
		learningRate(learningRate)
	{
		openblas_set_num_threads(threads);
		this->error = 1.0f;


		this->dataLength = 1 * weightRows;
		this->data = new float[dataLength];

		std::copy(data, data + dataLength, this->data);

		this->visibleRows = 1;
		this->visibleCols = weightRows;

		this->hiddenLength = visibleRows * weightsCols;
		this->hidden = new float[hiddenLength];
		this->visible = new float[visibleRows * visibleCols];

		this->weightsCols = weightsCols;
		weightsLength = weightRows * weightsCols;

		this->weights = new float[weightsLength];
		std::copy(weights, weights + weightsLength, this->weights);

		this->positive = new float[visibleCols * weightsCols];
		this->negative = new float[visibleCols * weightsCols];
	}

	void setData(float* data, int dataRows)
	{
		delete[] this->data;
		delete[] this->hidden;
		delete[] this->visible;

		this->dataLength = dataRows * visibleCols;
		this->data = new float[dataLength];

		std::copy(data, data + dataLength, this->data);

		this->visibleRows = dataRows;

		this->hiddenLength = visibleRows * weightsCols;
		this->hidden = new float[hiddenLength];
		this->visible = new float[visibleRows * visibleCols];
	}

	void setLearningRate(float learningRate)
	{
		this->learningRate = learningRate;
	}

	void setWeights(float* weights, int weightsRows, int weightsCols)
	{
		delete[] this->weights;
		this->visibleCols = weightsRows;
		this->weightsCols = weightsCols;
		weightsLength = visibleCols * weightsCols;

		this->weights = new float[weightsLength];
		std::copy(weights, weights + weightsLength, this->weights);
	}

	float* getVisible()
	{
		return visible;
	}

	float* runHidden(float* dataWithBias, int dataRows)
	{
		float* hidden = new float[dataRows * weightsCols];

		mmul(dataWithBias, dataRows, visibleCols, weights, weightsCols, hidden);
		applyLogistic(hidden, dataRows * weightsCols);
		return hidden;
	}

	float* runVisible(float* hidden, int dataRows)
	{
		float* visible = new float[dataRows * visibleCols];

		mmulTransposeB(hidden, dataRows, weightsCols, weights, visibleCols, visible);
		applyLogistic(visible, dataRows * visibleCols);
		return visible;
	}

	float* getWeights()
	{
		return weights;
	}

	float* getData()
	{
		return data;
	}

	float getError()
	{
		return error;
	}

	float getError(float * data, int rows) {
		float * hidden = runHidden(data, rows);
		float * visible = runVisible(hidden, rows);
		float error = updateError(rows * visibleCols, data, visible);
		delete[] hidden;
		delete[] visible;
		return error;
	}

	void trainWithoutError()
	{
		simpleTrain();
	}

	void trainBinarizedWithoutError()
	{
		binarizeHiddenTrain();
	}

	void trainWithError()
	{
		simpleTrain();
		this->error = updateError(dataLength, data, visible);
	}

	void trainBinarizedWithError()
	{
		binarizeHiddenTrain();
		this->error = updateError(dataLength, data, visible);
	}

	int getWeightsCols() {
		return weightsCols;
	}

	int getWeightsRows() {
		return visibleCols;
	}

	int getWeightsLength()
	{
		return weightsLength;
	}

	~RBM()
	{
		delete[] weights;
		delete[] data;
		delete[] hidden;
		delete[] visible;
		delete[] positive;
		delete[] negative;
	}

private:
	float error;
	float learningRate;

	float* weights;
	int weightsCols;
	int weightsLength;


	float* data;
	int visibleRows;
	int visibleCols;
	int dataLength;

	float* hidden;
	int hiddenRows;
	int hiddenLength;

	float* visible;

	float* positive;
	float* negative;



	inline void mmul(
		float* a, const unsigned int aRows, const unsigned int aColsbRows,
		float* b, const unsigned int bCols,
		float* c)
	{
		cblas_sgemm(
			CblasColMajor, CblasNoTrans, CblasNoTrans,
			aRows, bCols, aColsbRows, 1.0f,
			a, aRows, b, aColsbRows,
			0.0f, c, aRows);
	}

	inline void mmulTransposeB(
		float* a, const unsigned int aRows, const unsigned int aColsbCols,
		float* b, const unsigned int bRows,
		float* c)
	{
		cblas_sgemm(
			CblasColMajor, CblasNoTrans, CblasTrans,
			aRows, bRows, aColsbCols, 1.0f,
			a, aRows, b, bRows,
			0.0f, c, aRows);
	}

	inline void mmulTransposeA(
		float* a, const unsigned int aRowsbRows, const unsigned int aCols,
		float* b, const unsigned int bCols,
		float* c)
	{
		cblas_sgemm(
			CblasColMajor, CblasTrans, CblasNoTrans,
			aCols, bCols, aRowsbRows, 1.0f,
			a, aRowsbRows, b, aRowsbRows,
			0.0f, c, aCols);
	}


	inline void applyLogistic(float* data, const int size)
	{
#pragma omp parallel for
		for (int i = 0; i < size; i++)
		{
			data[i] = 1.0f / (expf(-data[i]) + 1.0f);
		}
	}

	inline void cd(float* dataA, float* dataB, float* result, float learningRate, const int size)
	{
#pragma omp parallel for
		for (int i = 0; i < size; i++)
		{
			result[i] += (dataA[i] - dataB[i]) * learningRate;
		}
	}

	inline void resetBias(float* data, int cols)
	{
		for (int i = 0; i < cols; i++)
		{
			data[i] = 1.0f;
		}
	}

	inline void simpleTrain()
	{

		//show(weights, visibleCols, weightsCols, "weights");

		//show(data, visibleRows, visibleCols, "data");
		// get hidden Probabilities
		mmul(data, visibleRows, visibleCols, weights, weightsCols, hidden);
		applyLogistic(hidden, hiddenLength);
		//show(hidden, visibleRows, weightsCols, "hidden");

		// positive Associations
		mmulTransposeA(data, visibleRows, visibleCols, hidden, weightsCols, positive);
		//show(positive, visibleCols, weightsCols, "positive");

		// visible Activations
		mmulTransposeB(hidden, visibleRows, weightsCols, weights, visibleCols, visible);
		applyLogistic(visible, dataLength);
		resetBias(visible, visibleRows);
		//show(visible, visibleRows, visibleCols, "visible");

		// get hidden Probabilities
		mmul(visible, visibleRows, visibleCols, weights, weightsCols, hidden);
		applyLogistic(hidden, hiddenLength);
		//show(hidden, visibleRows, weightsCols, "hidden 2");


		// negative Associations
		mmulTransposeA(visible, visibleRows, visibleCols, hidden, weightsCols, negative);
		//show(negative, visibleCols, weightsCols, "negative");

		cd(positive, negative, weights, learningRate / visibleRows, weightsLength);
	}

	inline void binarizeHiddenTrain()
	{
		//show(weights, visibleCols, weightsCols, "weights");
		//show(data, visibleRows, visibleCols, "data");

		// get hidden Probabilities
		mmul(data, visibleRows, visibleCols, weights, weightsCols, hidden);
		applyLogistic(hidden, hiddenLength);
		binarize(hidden, hiddenLength);
		//show(hidden, visibleRows, weightsCols, "hidden");

		// positive Associations
		mmulTransposeA(data, visibleRows, visibleCols, hidden, weightsCols, positive);
		//show(positive, visibleCols, weightsCols, "positive");

		// visible Activations
		mmulTransposeB(hidden, visibleRows, weightsCols, weights, visibleCols, visible);
		applyLogistic(visible, dataLength);
		resetBias(visible, visibleRows);
		//show(visible, visibleRows, visibleCols, "visible");

		// get hidden Probabilities
		mmul(visible, visibleRows, visibleCols, weights, weightsCols, hidden);
		applyLogistic(hidden, hiddenLength);
		//show(hidden, visibleRows, weightsCols, "hidden 2");


		// negative Associations
		mmulTransposeA(visible, visibleRows, visibleCols, hidden, weightsCols, negative);
		//show(negative, visibleCols, weightsCols, "negative");

		cd(positive, negative, weights, learningRate / visibleRows, weightsLength);
	}

	inline float updateError(int dataLength, float * data, float * visible)
	{
		float error = 0.0f;
#pragma omp parallel for reduction(+:error)
		for (int i = 0; i < dataLength; i++)
		{
			float diff = data[i] - visible[i];
			error += diff * diff;
		}
		error = sqrtf(error / (dataLength - visibleCols));

		return error;
		//std::cout << error << std::endl;
	}

	inline void binarize(float* data, int size) {
#pragma omp parallel
		{
			srand(int(time(NULL)) ^ omp_get_thread_num());

#pragma omp for
			for (int i = 0; i < size; i++)
			{
				data[i] = (data[i] > static_cast<float>(rand() % 1000) * 0.001f) ? 1.0f : 0.0f;
			}
		}
	}

	inline void show(float * data, int rows, int cols, char* name)
	{
		std::cout << std::fixed;
		std::cout << name << std::endl;
		for (int i = 0; i < cols; i++)
		{
			for (int j = 0; j < rows; j++)
			{
				std::cout << std::setprecision(5) << data[i * rows + j] << ", ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

};

#endif // RBM_H_

