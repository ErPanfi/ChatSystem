#include "Message.h"

#include "Socket.h" //for packing functions

int Message::pack(char buffer[]) const	
{
	char* bufferSentinel = buffer + Packable::pack(buffer);

	unsigned long netSendTime = Socket::host2network(m_sendingTime);
	memcpy(bufferSentinel, &netSendTime, sizeof(unsigned long));
	
	bufferSentinel += sizeof(unsigned long);


	for(int i = 0; i < MAX_MESSAGE_LEN && m_message[i] != '\0' && bufferSentinel - buffer < MAX_PACKET_SIZE - 1; ++i, ++bufferSentinel)
	{
		*bufferSentinel = m_message[i];
	}
	*bufferSentinel = '\0';

	//packet structure now is [PROTO_ID, PACKET_TYPE, sending_time, Message_with_delimiter]

	return bufferSentinel - buffer + 1;
}

void Message::unpack(char buffer[], int bufSize)
{
	char* bufferPtr = buffer + CHAT_PROTO_NBYTES + 1;	//skip first bytes, because already validated

	unsigned long netTime;
	memcpy(&netTime, bufferPtr, sizeof(unsigned long));
	m_sendingTime = Socket::network2host(netTime);
	bufferPtr += sizeof(unsigned long);

	m_message = t_message(bufferPtr);
}

