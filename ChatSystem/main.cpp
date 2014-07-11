//platform detection first

#include <iostream>
#include <time.h>
#include "ChatSystem.h"
#include "PlatformUtils.h"

const double FRAME_LEN = 1.0/60.0;

int main(int argc, char* argv[])
{
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

	if(!ChatSystem::initChatSystem(port))
	{
		std::cerr << "Cannot open socket on port " << port << std::endl;
		return -2;
	}

	port = atoi(argv[1]);
	
	//TODO now manually add other peer, implement automatic discovery
	ChatSystem::t_peerData peerData;
	strcpy(peerData.nickname, "The other one.");
	peerData.peerAddress.init(127,0,0,1,port);

	ChatSystem::addPeer(peerData);

	time_t lastFrameStart, currTime;

	bool doLoop = true;

	while(doLoop)
	{
		ChatSystem::t_message message;
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
				message.message = currPressedKey;
				ChatSystem::sendMessage(message);	
				break;
			}
		}

		if(ChatSystem::receiveMessage(message))
		{
			std::cout << message.message;
		}

		time(&currTime);
		double toSleep = FRAME_LEN - difftime(currTime, lastFrameStart);
		if(toSleep < FRAME_LEN && toSleep > 0)
		{
			PlatformUtils::waitForNextFrame(toSleep);
		}
	}

	std::cout << "Goodbye!" << std::endl;
	
	getchar();

	return 0;
}

