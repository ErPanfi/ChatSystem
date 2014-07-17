#include "MessageAck.h"
#include "Socket.h"
#include <string>

Packable::t_dataType MessageAck::getDataType() const
{
	return t_dataType::MessageAck;
}

int MessageAck::pack(char buffer[]) const
{
	char* bufferSentinel = buffer + Packable::pack(buffer);

	unsigned short mn = Socket::host2network(m_messageNum);
	memcpy(bufferSentinel, &mn, sizeof(unsigned short));
	bufferSentinel += sizeof(unsigned short);

	return bufferSentinel - buffer;
}

void MessageAck::unpack(char buffer[], int bufSize)
{
	unsigned short mn;
	memcpy(&mn, buffer + CHAT_PROTO_NBYTES + 1, sizeof(unsigned short));
	m_messageNum = Socket::network2host(mn);
}