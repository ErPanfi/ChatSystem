//platform detection first

#define PLATFORM_WINDOWS  1
#define PLATFORM_MAC      2
#define PLATFORM_UNIX     3

#if defined(_WIN32)
#define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define PLATFORM PLATFORM_MAC
#else
#define PLATFORM PLATFORM_UNIX
#endif

//then include correct libraries
#if PLATFORM == PLATFORM_WINDOWS
    #include <winsock2.h>

#elif PLATFORM == PLATFORM_MAC || 
        PLATFORM == PLATFORM_UNIX

		//this will be done if enough time remains
#endif

int main()
{
}