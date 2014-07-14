#include "Packable.h"

#include <string>

const char Packable::CHAT_PROTO_ID[CHAT_PROTO_NBYTES] = "MCS";

int Packable::validatePackage(char buffer[], int bufSize)
{
	if(bufSize < CHAT_PROTO_NBYTES + sizeof(t_dataType))	//too short!
	{
		return -1;
	}

	if(strcmp(buffer, CHAT_PROTO_ID) != 0)	//different protocol ID!
	{
		return -1;
	}

	return *((t_dataType*)(buffer + CHAT_PROTO_NBYTES));
}