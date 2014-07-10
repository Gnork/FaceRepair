#include "RBMUtils.h"

namespace RBMUtils
{
	RBM* initializeRBM(string weightsFile, int threads)
	{
		fstream infile(weightsFile, ios_base::in);
		cout << "start reading weights file" << endl;
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

		cout << "end reading weights file" << endl;
		infile.close();

		return new RBM(weights, rows, cols, threads);
	}
}
