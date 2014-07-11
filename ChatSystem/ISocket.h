#ifndef SOCKET_H
#define SOCKET_H

#include "Address.h"

class ISocket
{
protected:
	unsigned int m_socketHandle;
public:
	virtual bool open(unsigned short port) = 0;
	virtual void close() = 0;
	virtual bool isOpen() const = 0;
	virtual bool send(const Address& receiver, const void* data, int size) = 0;
	virtual int receive(Address& sender, void* data, int size) = 0;
	virtual bool initializeSocketSystem();
	virtual void cleanupSocketSystem();
	static const unsigned int MAX_PACKET_SIZE;
};

#endif