#include "Transmitter.h"
#include "DataManager.h"

#include<iostream>

Socket Transmitter::s_socket;
Socket::t_port Transmitter::s_port;

bool Transmitter::initTransmitter(Socket::t_port port)
{
	if(s_socket.initializeSocketSystem())
	{
		s_port = port;
		std::cout << "Listening on port " << port << std::endl;
		return s_socket.open(port);
	}
	else
	{
		return false;
	}
}

bool Transmitter::sendDataToPeers(Packable &data)
{
	char packetData[Packable::MAX_PACKET_SIZE];
	int packetSize = data.pack(packetData);

	for(DataManager::t_usersList::iterator uIter = DataManager::getUserIterator(); uIter != DataManager::getUserIteratorEnd(); ++uIter)
	{
		//TODO handle send failures
		s_socket.send( (*uIter) -> getAddress(), packetData, packetSize);
	}

	return true;
}

bool Transmitter::sendDataToAddress(Packable &data, Address address)
{
	char packetData[Packable::MAX_PACKET_SIZE];
	int packetSize = data.pack(packetData);

	return s_socket.send(address, packetData, packetSize);
}

bool Transmitter::sendBcastData(Packable &data)
{
	Address address;
	address.init(255,255,255,255, s_port);	//using global BCast address: routers don't forward these packets to the outer world
	char packetData[Packable::MAX_PACKET_SIZE];
	int packetSize = data.pack(packetData);

	return s_socket.send(address, packetData, packetSize);
}

Packable* Transmitter::receiveData()
{
	Address senderAddress;
	char packetData[Packable::MAX_PACKET_SIZE];
	int res;
	Message *messageData = NULL;
	User *userData = NULL;

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
			userData = new User(senderAddress);
			userData -> unpack(packetData, packetSize);
			DataManager::userDataReceived(userData);			
			break;
		default:	//unrecognized packet type
			return false;
		}
		
	}

	return false;
}