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
		char character;
	} s_packetPayload;

private:
	s_packetPayload m_payload;
public:
	inline s_packetPayload getPayload() const { return m_payload; }
	Packet(s_packetPayload payload) : m_payload(payload){}
};

#endif