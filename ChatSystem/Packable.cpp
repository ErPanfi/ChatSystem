#include "Packable.h"

#include <string>

const char Packable::CHAT_PROTO_ID[CHAT_PROTO_NBYTES] = "MCS";

int Packable::validatePackage(char buffer[], int bufSize)
{
	if(bufSize < CHAT_PROTO_NBYTES + 1)	//too short!
	{
		return -1;
	}

	if(strcmp(buffer, CHAT_PROTO_ID) != 0)	//different protocol ID!
	{
		return -1;
	}

	unsigned char ret = *(buffer + CHAT_PROTO_NBYTES);

	if(ret >= t_dataType::COUNT)	//unrecognized type
	{
		return -1;
	}
	else
	{
		return ret;
	}
}

int Packable::pack(char buffer[]) const
{
	char* bufferSentinel = buffer;
	strcpy_s(bufferSentinel, CHAT_PROTO_NBYTES, CHAT_PROTO_ID);

	*(bufferSentinel += CHAT_PROTO_NBYTES) = (char)getDataType();

	return bufferSentinel - buffer + 1;
}