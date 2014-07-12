#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "PlatformDetection.h"

class PlatformUtils
{
public:
	static const char NO_CHAR_READ = '\0';
	static char currKeyPressed();
	static void waitForNextFrame(double seconds);
};

#endif