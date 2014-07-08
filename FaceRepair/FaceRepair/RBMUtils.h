#pragma once
#include "RBM.h"
#include <iostream>
#include <fstream>

using namespace std;

namespace RBMUtils
{
	RBM* initializeRBM(string weightsFile, int threads);
}

