
#include "Socket.h"
#include "PlatformDetection.h"

//this can compile only under Win
#if PLATFORM == PLATFORM_WIN
#include <WinSock2.h>
#endif

bool Socket::initializeSocketSystem()
{
#if PLATFORM == PLATFORM_WIN
		WSADATA wsaData;
		return (WSAStartup(MAKEWORD(2,2), &wsaData) == NO_ERROR);	//using socket version 2.2 ... why this version? No idea ^.^'
#else
	return true;
#endif
}

void Socket::cleanupSocketSystem()
{
#if PLATFORM == PLATFORM_WIN
	WSACleanup();
#endif
}

Socket::Socket()
	//TODO : m_socketHandle(INVALID_SOCKET) doesn't work... why?
{
	m_socketHandle = INVALID_SOCKET;
}

bool Socket::open(unsigned short port)
{
	m_socketHandle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if(m_socketHandle == INVALID_SOCKET)	//should never happen if WSAStartup has succedeed
	{
		return false;
	}

	//bind socket to port
	sockaddr_in inAddress;
	inAddress.sin_family = AF_INET;
	inAddress.sin_addr.S_un.S_addr = INADDR_ANY;
	inAddress.sin_port = htons(port);

	if(bind(m_socketHandle, (const sockaddr*) &inAddress, sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		return false;
	}

	//setting socket as non-blocking
	DWORD nb = 1;
	if(ioctlsocket(m_socketHandle, FIONBIO, &nb) != NO_ERROR)	
	{
		return false;
	}

	return true;
}

void Socket::close()
{
#if PLATFORM == PLATFORM_WIN
	closesocket(m_socketHandle);
#endif
	m_socketHandle = INVALID_SOCKET;
}

bool Socket::isOpen() const
{
	return m_socketHandle != INVALID_SOCKET;
}

bool Socket::send(const Address& receiver, const void* data, int size)
{
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.S_un.S_addr = htonl(receiver.getAddress());
	address.sin_port = htons(receiver.getPort());

	int bytesSent = sendto(m_socketHandle, (const char*) data, size, 0, (const sockaddr*) &address, sizeof(sockaddr_in));

	return bytesSent == size;
}

int Socket::receive(Address& sender, void* data, int size)
{
	sockaddr_in fromAddress;
	int fromLen = sizeof(sockaddr_in);
	int receivedBytes = recvfrom(m_socketHandle, (char*) data, size, 0, (sockaddr*) &fromAddress, &fromLen);

	if(receivedBytes > 0)
	{
		sender.init(ntohl(fromAddress.sin_addr.S_un.S_addr), ntohs(fromAddress.sin_port));
	}
	else
	{
#if PLATFORM == PLATFORM_WIN
		receivedBytes = WSAGetLastError();
		switch (receivedBytes)
		{
		case WSAEWOULDBLOCK:	//this is not an error: socket has no data, so it would block execution, but it can't because it's unblocking
			receivedBytes = 0;
			break;
		}
#endif
	}

	return receivedBytes;
}