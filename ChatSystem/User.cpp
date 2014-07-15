#include "User.h"

int User::pack(char buffer[]) const	
{
	char* bufferSentinel = buffer;
	strcpy_s(bufferSentinel, CHAT_PROTO_NBYTES, CHAT_PROTO_ID);

	*(bufferSentinel += CHAT_PROTO_NBYTES) = (char)t_dataType::UserType;
	++bufferSentinel;


	//we don't send address over the internet, it will be contained in UDP packet

	for(int i = 0; i < MAX_NICK_LEN && m_nick[i] != '\0' && bufferSentinel - buffer < MAX_PACKET_SIZE; ++i, ++bufferSentinel)
	{
		*bufferSentinel = m_nick[i];
	}

	*bufferSentinel = '\0';

	//packet structure now is [PROTO_ID, PACKET_TYPE, Nick_with_delimiter]

	return bufferSentinel - buffer + 1;
}

void User::unpack(char buffer[], int bufSize)
{
	int i, j;
	//if we're at this point package has already been validated
	for(j = 0, i = CHAT_PROTO_NBYTES + 1; i < bufSize && j < MAX_NICK_LEN && buffer[i] != '\0'; ++i, ++j)
	{
		m_nick[j] = buffer[i];
	}

	m_nick[j] = '\0';
}

void User::refreshFromOther(User &other)
{
	setNick(other.m_nick);
}

std::string User::toString() const
{
	return std::string(m_nick) + "(" + m_address.toString() + ")";
}