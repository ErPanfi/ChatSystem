
#include "WinSocket.h"

//this can compile only under Win
#if PLATFORM == PLATFORM_WIN

bool WinSocket::initializeSocketSystem()
{
		WSADATA wsaData;
		return (WSAStartup(MAKEWORD(2,2), &wsaData) == NO_ERROR);	//using socket version 2.2 ... why this version? No idea ^.^'
}

void WinSocket::cleanupSocketSystem()
{
	WSACleanup();
}

WinSocket::WinSocket()
	//TODO : m_socketHandle(INVALID_SOCKET) doesn't work... why?
{
	m_socketHandle = INVALID_SOCKET;
}

bool WinSocket::open(unsigned short port)
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

void WinSocket::close()
{
	closesocket(m_socketHandle);
	m_socketHandle = INVALID_SOCKET;
}

bool WinSocket::isOpen() const
{
	return m_socketHandle != INVALID_SOCKET;
}

bool WinSocket::send(const Address& receiver, const void* data, int size)
{
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.S_un.S_addr = htonl(receiver.getAddress());
	address.sin_port = htons(receiver.getPort());

	int bytesSent = sendto(m_socketHandle, (const char*) data, size, 0, (const sockaddr*) &address, sizeof(sockaddr_in));

	return bytesSent == size;
}

int WinSocket::receive(Address& sender, void* data, int size)
{
	sockaddr_in fromAddress;
	int fromLen = sizeof(sockaddr_in);
	int receivedBytes = recvfrom(m_socketHandle, (char*) data, size, 0, (sockaddr*) &fromAddress, &fromLen);

	if(receivedBytes > 0)
	{
		sender.init(ntohl(fromAddress.sin_addr.S_un.S_addr), ntohs(fromAddress.sin_port));
	}

	return receivedBytes;
}

#endif	//platform check