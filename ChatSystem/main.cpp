//platform detection first

#include <iostream>
#include <time.h>
#include "Transmitter.h"
#include "DataManager.h"
#include "PlatformUtils.h"

const double FPS_CAP = 60.0;
const double FRAME_LEN = 1.0/FPS_CAP;

const Socket::t_port DEFAULT_PORT = 30000;

//this is separate in order to drop automatically some temporary variables from the stack once init has finished
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

	//other initializations
	PlatformUtils::init();

	//clear stdin
	std::cout << "Starting chat..." << std::endl;
	getchar();
	std::cin.clear();

	return 0;
}

int main(int argc, char* argv[])
{
	short retCode;
	if((retCode = initFromArgs(argc, argv)) < 0)	//this will dump temporary variables fromt the stack once init step is finished
	{
		getchar();
		return retCode;
	}

	time_t thisFrameStart, lastFrameStart, currTime;

	PlatformUtils::printFooter();
	time(&lastFrameStart);

	bool doLoop = true;
	while(doLoop)
	{
		//mark frame start
		time(&thisFrameStart);

		//confront elapsed time from last frame
		double elapsed = difftime(thisFrameStart, lastFrameStart);	//first iteration will be of a few milliseconds

		//single components update
		DataManager::update(elapsed);
		Transmitter::update(elapsed);

		//parse input
		char currPressedKey = PlatformUtils::currKeyPressed();
		if(currPressedKey != PlatformUtils::NO_CHAR_READ)
		{
			//which key pressed?
			switch (currPressedKey)
			{
			case 27:	//ESC key
				doLoop = false;
				break;

			case 'U':
			case 'u':
				std::cout << std::endl << std::endl;
				DataManager::printUsers();
				PlatformUtils::printFooter();
				break;

			case 'M':
			case 'm':
				DataManager::writeNewMessage();	//print all messages after input
			case 'R':
			case 'r':
				std::cout << std::endl << std::endl;
				DataManager::printMessages();
				PlatformUtils::printFooter();
				break;
			case '?':
				std::cout << std::endl << std::endl;
				PlatformUtils::printCommandList();
				PlatformUtils::printFooter();
				break;
			}
		}


		//use this frame start as last frame start for elapsed time calculation
		lastFrameStart = thisFrameStart;

		time(&currTime);
		//how much to sleep
		double toSleep = FRAME_LEN - difftime(currTime, thisFrameStart);
		if(toSleep < FRAME_LEN && toSleep > 0)	//don't ever sleep for more than a frame, or for a negative time
		{
			PlatformUtils::waitForNextFrame(toSleep);
		}
	}

	std::cout << "Goodbye!" << std::endl;

	//cleanup step
	DataManager::cleanup();
	Transmitter::cleanup();
	
	
	getchar();

	return 0;
}

