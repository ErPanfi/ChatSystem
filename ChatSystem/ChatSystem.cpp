#include "ChatSystem.h"

ChatSystem::t_peerData ChatSystem::s_peerList[MAX_PEER_LIST_SIZE];
unsigned short ChatSystem::s_nPeer = 0;
Socket ChatSystem::s_socket;


bool ChatSystem::initChatSystem(unsigned short port)
{
	s_socket.initializeSocketSystem();
	return s_socket.open(port);
}

bool ChatSystem::sendMessage(t_message message)
{
	t_packetPayload packetPayload;
	packetPayload.type = e_packetType::Message;
	packetPayload.message = message;

	for (unsigned short i = 0; i < ChatSystem::s_nPeer; ++i)
	{
		s_socket.send(ChatSystem::s_peerList[i].peerAddress, &packetPayload, sizeof(t_packetPayload));
	}

	//TODO handle resends
	return true;
}

void ChatSystem::addPeer(t_peerData peerData)
{
	ChatSystem::s_peerList[ChatSystem::s_nPeer++] = peerData;
}

bool ChatSystem::receiveMessage(t_message &message)
{
	t_packetPayload packetPayload;
	Address senderAddress;

	if(s_socket.receive(senderAddress, &packetPayload, sizeof(packetPayload)))
	{
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

	return false;
}