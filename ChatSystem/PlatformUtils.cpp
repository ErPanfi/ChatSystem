#include "PlatformUtils.h"

#if PLATFORM==PLATFORM_WIN
#include <Windows.h>
#include <conio.h>
#endif

#include <iostream>
#include <time.h>

time_t PlatformUtils::s_timeRef;

void PlatformUtils::init()
{
	struct tm timeStamp;
	memset(&timeStamp, 0, sizeof(timeStamp));
	timeStamp.tm_year = 2014 - 1900;

	s_timeRef = mktime(&timeStamp);	//time reference is 01/01/2014
}

PlatformUtils::t_relativeTime PlatformUtils::time2relative(time_t absTime)
{
	if(!absTime)
	{
		time(&absTime);
	}

	return (t_relativeTime) difftime(absTime, s_timeRef);
}

// returns 0 if no key pressed
char PlatformUtils::currKeyPressed()
{
#if PLATFORM == PLATFORM_WIN
	if(_kbhit())
	{
		return _getch();
	}
#endif
	return 0;
}

void PlatformUtils::waitForNextFrame(double seconds)
{
#if PLATFORM == PLATFORM_WIN
		Sleep((unsigned int)(seconds*1000));
#endif
}

void PlatformUtils::log(std::string message)
{
	std::cout << message << std::endl;
}

void PlatformUtils::printFooter()
{
	std::cout << "Type command key (\'?\' for command list) : ";
}

void PlatformUtils::printCommandList()
{
	std::cout	<< "'U' : print connected users list" << std::endl //
				<< "'M' : write a new message (blocks packets reception until input completed)"	<< std::endl //
				<< "\'R\' print last messages received" << std::endl //
				<< "\'?\' : print this command list" << std::endl //
				<< " \'Esc\' : Quit" << std::endl;
}