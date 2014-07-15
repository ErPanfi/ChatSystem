#include "Message.h"

int Message::pack(char buffer[]) const	
{
	char* bufferSentinel = buffer;
	strcpy_s(bufferSentinel, CHAT_PROTO_NBYTES, CHAT_PROTO_ID);

	*(bufferSentinel += CHAT_PROTO_NBYTES) = (char)t_dataType::MessageType;
	++bufferSentinel;

	for(int i = 0; i < MAX_MESSAGE_LEN && m_message[i] != '\0' && bufferSentinel - buffer < MAX_PACKET_SIZE; ++i, ++bufferSentinel)
	{
		*bufferSentinel = m_message[i];
	}
	*bufferSentinel = '\0';

	//packet structure now is [PROTO_ID, PACKET_TYPE, Message_with_delimiter]

	return bufferSentinel - buffer + 1;
}

void Message::unpack(char buffer[], int bufSize)
{
	int i, j;
	//if we're at this point package has already been validated
	for(j = 0, i = CHAT_PROTO_NBYTES + 1; i < bufSize && j < MAX_MESSAGE_LEN && buffer[i] != '\0'; ++i, ++j)
	{
		m_message[j] = buffer[i];
	}

	m_message[j] = '\0';
}