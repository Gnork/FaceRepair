#include <iostream>
#include "WebcamHandler.h"

using namespace std;

int main()
{
	WebcamHandler webcam = WebcamHandler(1280, 720, 64, 16);
	webcam.run();
	return 0;
}

