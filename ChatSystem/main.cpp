//platform detection first

#include <iostream>
#include <time.h>
#include "ChatSystem.h"
#include "PlatformUtils.h"

const double FRAME_LEN = 1.0/30.0;

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		std::cout << "Missing port number!!!" << std::endl;
		return -1;
	}
	else if (argc < 3)
	{
		std::cout << "Missing target port number!!!" << std::endl;
		return -1;
	}

	unsigned short port = atoi(argv[1]);

	if(!ChatSystem::initChatSystem(port))
	{
		std::cerr << "Cannot open socket on port " << port << std::endl;
		return -2;
	}

	port = atoi(argv[2]);
	
	//TODO now manually add other peer, implement automatic discovery
	ChatSystem::t_peerData peerData;
	strcpy_s(peerData.nickname, "The other one.");
	peerData.peerAddress.init(127,0,0,1,port);

	ChatSystem::addPeer(peerData);

	time_t lastFrameStart, currTime;

	bool doLoop = true;

	while(doLoop)
	{
		ChatSystem::t_message message;
		time(&lastFrameStart);
		char currPressedKey = PlatformUtils::currKeyPressed();
		if(currPressedKey != PlatformUtils::NO_CHAR_READ)
		{
			switch (currPressedKey)
			{
			case 27:	//ESC key pressed
				doLoop = false;
				break;
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
				
				if(!ChatSystem::sendMessage(message))
				{
					std::cerr << "Failed to send message :(" << currPressedKey << std::endl;
				}
				break;
			}
		}

		if(ChatSystem::receiveMessage(message))
		{
			if(message.message != '\n')
			{
				std::cout << message.message;
			}
			else
			{
				std::cout << std::endl;
			}
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

