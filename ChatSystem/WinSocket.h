#include "PlatformDetection.h"

#if PLATFORM == PLATFORM_WIN

#ifndef WINSOCKET_H
#define WINSOCKET_H

#include <WinSock2.h>
#include "ISocket.h"

class WinSocket : public ISocket
{
public:
	WinSocket();
	virtual bool open(unsigned short port);
	virtual void close();
	virtual bool isOpen() const;
	virtual bool send(const Address& receiver, const void* data, int size);
	virtual int receive(Address& sender, void* data, int size);
	virtual bool initializeSocketSystem();
	virtual void cleanupSocketSystem();
};

typedef WinSocket Socket;	//now I can use "Socket" in whole code, if Windows

#endif	//WINSOCKET_H now defined

#endif	//platform check