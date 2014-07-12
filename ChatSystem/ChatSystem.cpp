#include "ChatSystem.h"

#include<iostream>

ChatSystem::t_peerData ChatSystem::s_peerList[MAX_PEER_LIST_SIZE];
unsigned short ChatSystem::s_nPeer = 0;
Socket ChatSystem::s_socket;


bool ChatSystem::initChatSystem(unsigned short port)
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

bool ChatSystem::sendMessage(t_message message)
{
	t_packetPayload packetPayload;
	packetPayload.type = e_packetType::Message;
	packetPayload.message = message;

	for (unsigned short i = 0; i < ChatSystem::s_nPeer; ++i)
	{
		Address::t_addressStr addressStr;
		s_peerList[i].peerAddress.getAddressStr(addressStr);
		s_socket.send(s_peerList[i].peerAddress, &packetPayload, sizeof(t_packetPayload));
	}

	//TODO handle resends
	return true;
}

void ChatSystem::addPeer(t_peerData peerData)
{
	s_peerList[s_nPeer++] = peerData;
	Address::t_addressStr addressStr;
	peerData.peerAddress.getAddressStr(addressStr) ;
	std::cout << "Added " << addressStr << " to peer list." << std::endl;
}

bool ChatSystem::receiveMessage(t_message &message)
{
	t_packetPayload packetPayload;
	Address senderAddress;

	//if(s_socket.receive(senderAddress, &packetPayload, sizeof(t_packetPayload))>0)
	int returnValue = s_socket.receive(senderAddress, &packetPayload, sizeof(t_packetPayload));
	if(returnValue > 0)
	{
		Address::t_addressStr addressStr;
		senderAddress.getAddressStr(addressStr);
		switch (packetPayload.type)
		{
		case e_packetType::Message:
			message = packetPayload.message;
			return true;
		case e_packetType::PeerData:
			addPeer(packetPayload.peerData);
			break;
		}
	}
	else if(returnValue == 0)
	{
		std::cout << "Remote connection closed gracefully" << std::endl;
	}
	else if((returnValue = WSAGetLastError()) != WSAEWOULDBLOCK)
	{
		std::cerr << "Error occured on receiving : " << returnValue << std::endl;
	}

	return false;
}