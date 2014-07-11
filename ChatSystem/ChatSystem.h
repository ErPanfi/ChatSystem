#ifndef CHAT_SYSTEM_H
#define CHAT_SYSTEM_H

#include "Socket.h"


class ChatSystem
{

public:
	static const unsigned short MAX_NICKNAME_LEN = 100;

	typedef struct PeerDataStruct
	{
		Address peerAddress;
		char nickname[MAX_NICKNAME_LEN];
	} t_peerData;

	typedef struct MessageStruct
	{
		char message;
	} t_message;

	typedef enum PacketTypeEnum
	{
		PeerData = 0,
		Message,
	} e_packetType;

	typedef struct PacketPayloadStruct
	{
		e_packetType type;
		union //anonymous union with packet payload
		{
			t_message message;
			t_peerData peerData;
		};
	} t_packetPayload;


private:
	static const unsigned short MAX_PEER_LIST_SIZE = 10;
	static t_peerData s_peerList[MAX_PEER_LIST_SIZE];
	static unsigned short s_nPeer;

	static Socket s_socket;
public:	
	static bool initChatSystem(unsigned short port);
	static bool sendMessage(t_message message);
	static bool receiveMessage(t_message &message);
	static void addPeer(t_peerData peerData);
};


#endif