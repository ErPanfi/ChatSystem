#ifndef PLATFORM_UTILS_H
#define PLATFORM_UTILS_H

#include "PlatformDetection.h"
#include <string>

//this class wraps every non-network function that is strictly platform-dependant, in order to hide platform type to the rest of code
class PlatformUtils
{
private:
	static time_t s_timeRef;

public:
	static const char NO_CHAR_READ = '\0';
	//returns the current pressed key on keyboard, or NO_CHAR_READ if no key pressed
	static char currKeyPressed();
	//used to put thread on sleep
	static void waitForNextFrame(double seconds);
	//single log method. TODO: implement logging types (warning, errors) and levels, for a more controlled logging
	static void log(std::string message);

	static void init();
	typedef unsigned long t_relativeTime;

	//convert an absolute time to a relative time, in order to ease the network-related operations with it
	static t_relativeTime time2relative(time_t absTime = 0);
	//convert from relative time to human readable time
	static std::string relativeTime2str(t_relativeTime rel);

	//print the user interface footer
	static void printFooter();
	//print the accepted command list
	static void printCommandList();
};

#endif	//PLATFORM_UTILS_H