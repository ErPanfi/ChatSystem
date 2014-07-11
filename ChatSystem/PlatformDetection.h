//if platform already detected don't repeat yourself
#ifndef PLATFORM

//possible platforms
#define PLATFORM_WIN	  1
#define PLATFORM_MAC      2
#define PLATFORM_UNIX     3

//detection of current platform
#if defined(_WIN32)
#define PLATFORM PLATFORM_WIN
#elif defined(__APPLE__)
#define PLATFORM PLATFORM_MAC
#else
#define PLATFORM PLATFORM_UNIX
#endif

#endif	//PLATFORM now defined