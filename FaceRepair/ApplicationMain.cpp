#include <iostream>
#include "WebcamHandler.h"

using namespace std;

int main()
{
	WebcamHandler* webcam = new WebcamHandler();
	webcam->run();
	return 0;
}

