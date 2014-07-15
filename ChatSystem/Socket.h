#ifndef SOCKET_H
#define SOCKET_H

#include "Address.h"

///This will wrap the platform-dependent socket code
class Socket
{
protected:
	unsigned int m_socketHandle;
public:
	typedef unsigned short t_port;
	Socket();
	bool open(t_port port);
	void close();
	bool isOpen() const;
	bool send(const Address& receiver, const void* data, int size);
	int receive(Address& sender, void* data, int size);
	bool initializeSocketSystem();
	void cleanupSocketSystem();
};

#endif	//SOCKET_H now defined