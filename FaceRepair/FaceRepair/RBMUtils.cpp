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

	RBM* initializeRBM(string sizeFile, string binFile, int threads)
	{
		fstream infile(sizeFile, ios_base::in);
		string line;
		int rows, cols;
		infile >> rows;
		infile >> cols;
		infile.close();

		int len = rows * cols;
		float* weights = new float[len]();
		ifstream testfile(binFile, ios::in | ios::binary);
		testfile.read((char*)weights, sizeof(float)*len);
		infile.close();

		return new RBM(weights, rows, cols, threads);
	}
}
