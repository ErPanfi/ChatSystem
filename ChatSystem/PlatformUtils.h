#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "PlatformDetection.h"

class PlatformUtils
{
public:
	static char currKeyPressed();
	static void waitForNextFrame(double seconds);
};

#endif