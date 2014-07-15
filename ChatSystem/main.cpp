//platform detection first

#include <iostream>
#include <time.h>
#include "Transmitter.h"
#include "DataManager.h"
#include "PlatformUtils.h"

const double FPS_CAP = 10.0;
const double FRAME_LEN = 1.0/FPS_CAP;

const Socket::t_port DEFAULT_PORT = 30000;


short initFromArgs(int argc, char* argv[])
{
	Socket::t_port port = DEFAULT_PORT;

	if(argc > 1)
	{
		port = atoi(argv[1]);	
	}

	if(!Transmitter::initTransmitter(port))
	{
		std::cerr << "Cannot open socket on port " << port << std::endl;
		return -1;
	}

	User::t_nickType myNick;

	if (argc > 2)
	{
		myNick = argv[1];
	}
	else
	{
		std::cout << "Insert nickname (" << User::MAX_NICK_LEN - 1 <<" chars max) : ";
		std::cin >> myNick;
	}

	DataManager::getCurrUser() -> setNick(myNick);

	return 0;
}

int main(int argc, char* argv[])
{
	short retCode;
	if((retCode = initFromArgs(argc, argv)) < 0)	//this will dump temporary variables once init step is finished
	{
		getchar();
		return retCode;
	}

	time_t thisFrameStart, lastFrameStart, currTime;

	time(&lastFrameStart);

	bool doLoop = true;
	while(doLoop)
	{
		time(&thisFrameStart);

		double elapsed = difftime(thisFrameStart, lastFrameStart);

		//here goes components update
		DataManager::update(elapsed);
		Transmitter::update(elapsed);

		char currPressedKey = PlatformUtils::currKeyPressed();
		if(currPressedKey != PlatformUtils::NO_CHAR_READ)
		{
			switch (currPressedKey)
			{
			case 27:	//ESC key pressed
				doLoop = false;
				break;

			case 'U':
			case 'u':
				DataManager::printUsers();
				break;

				/*
			default:
				
				if(currPressedKey != 13)
				{
					message.message = currPressedKey;
					std::cout << currPressedKey;
				}
				else
				{
					message.message = '\n';
					std::cout << std::endl;
				}
				
				if(!Transmitter::sendMessage(message))
				{
					std::cerr << "Failed to send message :(" << currPressedKey << std::endl;
				}
				break;
				*/
			}
		}

		lastFrameStart = thisFrameStart;

		time(&currTime);
		double toSleep = FRAME_LEN - difftime(currTime, thisFrameStart);
		if(toSleep < FRAME_LEN && toSleep > 0)
		{
			PlatformUtils::waitForNextFrame(toSleep);
		}
	}

	std::cout << "Goodbye!" << std::endl;
	
	getchar();

	return 0;
}

