#ifndef PACKET_H
#define PACKET_H

#include "Address.h"

class Packet
{
public:
	typedef enum PacketTypeEnum
	{
		Exists = 0,
		Message,
	} e_packetType;

	typedef struct PacketPayloadStruct
	{
		e_packetType packetType;
	} s_packetPayload;


};

#endif