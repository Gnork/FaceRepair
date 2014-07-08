#include "RBMUtils.h"

namespace RBMUtils
{
	RBM* initializeRBM(string weightsFile, int threads)
	{
		ifstream infile(weightsFile);
		string line;
		int rows, cols;
		infile >> rows;
		infile >> cols;

		float* weights = new float[rows*cols]();

		float val;
		int i = 0;
		while (infile >> val)
		{
			weights[i++] = val;
		}

		return new RBM(weights, rows, cols, threads);
	}
}
