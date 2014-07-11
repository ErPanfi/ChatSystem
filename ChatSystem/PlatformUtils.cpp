#include "PlatformUtils.h"

#if PLATFORM==PLATFORM_WIN
#include <conio.h>
#include <dos.h>
#endif

// returns 0 if no key pressed
char PlatformUtils::currKeyPressed()
{
#if PLATFORM == PLATFORM_WIN
	if(kbhit())
	{
		return getch();
	}
#endif
	return 0;
}

void PlatformUtils::sleep(double seconds)
{
	if(seconds > 0)
	{
#if PLATFORM == PLATFORM_WIN
	sleep(seconds);
#endif
	}
}