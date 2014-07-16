#ifndef PLATFORM_UTILS_H
#define PLATFORM_UTILS_H

#include "PlatformDetection.h"
#include <string>

class PlatformUtils
{
private:
	static time_t s_timeRef;

public:
	static const char NO_CHAR_READ = '\0';
	static char currKeyPressed();
	static void waitForNextFrame(double seconds);
	static void log(std::string message);

	typedef unsigned long t_relativeTime;
	static void init();
	static t_relativeTime time2relative(time_t absTime = 0);

	static void printFooter();
	static void printCommandList();
};

#endif	//PLATFORM_UTILS_H