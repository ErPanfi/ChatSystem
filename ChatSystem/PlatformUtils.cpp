#include "PlatformUtils.h"

#if PLATFORM==PLATFORM_WIN
#include <Windows.h>
#include <conio.h>
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

void PlatformUtils::waitForNextFrame(double seconds)
{
#if PLATFORM == PLATFORM_WIN
		Sleep((unsigned int)(seconds*1000));
#endif
}


