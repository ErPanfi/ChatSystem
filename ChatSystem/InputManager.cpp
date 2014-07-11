#include "InputManager.h"

#if PLATFORM==PLATFORM_WIN
#include <conio.h>
#endif

// returns 0 if no key pressed
char InputManager::currKeyPressed() const
{
#if PLATFORM == PLATFORM_WIN
	if(kbhit())
	{
		return getch();
	}
#endif
	return 0;
}