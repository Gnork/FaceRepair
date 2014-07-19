#include <iostream>
#include <fstream>

using namespace std;

int main()
{

	cout << "reading weights file" << endl;
	fstream infile("weights\\WildFaces_64x64_rgb_2kh_25000it.out", ios_base::in);
	if (!infile)
	{
		cout << "Cannot open infile.";
		return 1;
	}

	string line;
	int rows, cols;
	infile >> rows;
	infile >> cols;

	int len = rows * cols;

	float* weights = new float[len]();

	float val;
	int i = 0;
	while (infile >> val)
	{
		weights[i++] = val;
	}

	infile.close();
	cout << "end reading weights file" << endl;

	for (int i = 0; i < 10; ++i)
	{
		cout << weights[i] << endl;
	}


	cout << "start writing bin file" << endl;
	ofstream outfile("weights\\WildFaces_64x64_rgb_2kh_25000it.bin", ios::out | ios::binary);
	outfile.write((char *)weights, sizeof(float) * len);
	outfile.close();
	cout << "end writing bin file" << endl;



	/*
	cout << "start reading bin file" << endl;
	float* test = new float[len]();

	ifstream testfile("weights\\WildFaces_64x64_rgb_1,5kh_104000it.bin", ios::in | ios::binary);
	testfile.read((char*)test, sizeof(float)*len);
	infile.close();
	cout << "end reading bin file" << endl;



	for (int i = 0; i < len; ++i){
		if (weights[i] != test[i])
		{
			cout << i << ": " << weights[i] << " != " << test[i];
		}
	}
	delete test;
	*/

	delete weights;
	

	return 0;
}