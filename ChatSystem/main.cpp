//platform detection first

#include "Socket.h"
#include <iostream>
#include "PlatformUtils.h"
#include <time.h>

const double FRAME_LEN = 1.0/60.0;

int main(int argc, char* argv[])
{
	/*
	if(argc < 1)
	{
		std::cout << "Missing port number!!!" << std::endl;
		return -1;
	}
	else if (argc < 2)
	{
		std::cout << "Missing target port number!!!" << std::endl;
		return -1;
	}

	unsigned short port = atoi(argv[0]);
	*/

	time_t lastFrameStart, currTime;

	bool doLoop = true;

	while(doLoop)
	{
		time(&lastFrameStart);
		char currPressedKey;
		if(currPressedKey = PlatformUtils::currKeyPressed())
		{
			switch (currPressedKey)
			{
			case 27:	//ESC key pressed
				doLoop = false;
				break;
			default:
				
				break;
			}
		}

		time(&currTime);
		PlatformUtils::sleep(difftime(currTime, lastFrameStart));
	}

	std::cout << "Goodbye!" << std::endl;
	
	getchar();

	return 0;
}

