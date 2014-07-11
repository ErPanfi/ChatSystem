#include "PlatformDetection.h"

#if PLATFORM == PLATFORM_WIN
#include "WinSocket.h"
#elif PLATFORM == PLATFORM_MAC
#else
#endif