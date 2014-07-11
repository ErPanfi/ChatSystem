#include "ISocket.h"

const unsigned int ISocket::MAX_PACKET_SIZE = 256;

bool ISocket::initializeSocketSystem()
{
	return true;
}

void ISocket::cleanupSocketSystem()
{
	//do nothing
}