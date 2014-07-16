#include "Message.h"

int Message::pack(char buffer[]) const	
{
	char* bufferSentinel = buffer;
	strcpy_s(bufferSentinel, CHAT_PROTO_NBYTES, CHAT_PROTO_ID);

	*(bufferSentinel += CHAT_PROTO_NBYTES) = (char)t_dataType::MessageType;
	++bufferSentinel;

	*((PlatformUtils::t_relativeTime*)bufferSentinel) = m_sendingTime;		//TODO pack this with htond
	bufferSentinel += sizeof(PlatformUtils::t_relativeTime)/sizeof(char);	


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
	char* bufferSentinel = buffer + CHAT_PROTO_NBYTES + 1;	//skip first bytes, because already validated

	m_sendingTime = *((PlatformUtils::t_relativeTime*)bufferSentinel);		//TODO unpack this with ntohd
	bufferSentinel += sizeof(PlatformUtils::t_relativeTime)/sizeof(char);

	m_message = std::string(bufferSentinel);
}

