#include "Transmitter.h"
#include "DataManager.h"

#include<iostream>

Socket Transmitter::s_socket;

bool Transmitter::initTransmitter(unsigned short port)
{
	if(s_socket.initializeSocketSystem())
	{
		std::cout << "Listening on port " << port << std::endl;
		return s_socket.open(port);
	}
	else
	{
		return false;
	}
}

bool Transmitter::sendData(Packable &data)
{
	char packetData[Packable::MAX_PACKET_SIZE];
	int packetSize = data.pack(packetData);


	//TODO handle resends
	return true;
}

Packable* Transmitter::receiveData()
{
	Address senderAddress;
	char packetData[Packable::MAX_PACKET_SIZE];
	int res;
	Message *messageData = NULL;
	User *user = NULL;

	//if(s_socket.receive(senderAddress, &packetPayload, sizeof(t_packetPayload))>0)
	int packetSize = s_socket.receive(senderAddress, packetData, Packable::MAX_PACKET_SIZE);
	if(packetSize > 0 && (res = Packable::validatePackage(packetData, packetSize)) >= 0)
	{
		switch (res)
		{
		case Packable::t_dataType::MessageType:
			messageData = new Message();
			messageData  -> unpack(packetData, packetSize);
			DataManager::messageReceived(senderAddress, messageData);
			break;
		case Packable::t_dataType::UserType:
			user = new User();
			user -> unpack(packetData, packetSize);
			DataManager::userDataReceived(senderAddress, user);

			log("New user!");
			
			break;
		default:	//unrecognized packet type
			return false;
		}
		
	}

	return false;
}

void Transmitter::log(std::string message)
{
	std::cout << message << std::endl;
}

