#ifndef SOCKET_H
#define SOCKET_H

#include "Address.h"

///This will wrap the platform-dependent socket code
class Socket
{
private:
	unsigned int m_socketHandle;
public:
	typedef unsigned short t_port;
	Socket();

	//open the socket and bind it to given port
	bool open(t_port port);	
	//close the socket
	void close();
	//check if socket is open
	bool isOpen() const;
	//send data throug socket
	bool send(const Address& receiver, const void* data, int size);
	//receive data through socket
	int receive(Address& sender, void* data, int size);
	//this is mandatory in win, should be called at startup
	bool initializeSocketSystem();
	//mandatory in win, should be called at program termination
	void cleanupSocketSystem();

	/**
	 * some conversion methods...
	 */

	static unsigned long host2network(long l);
	static long network2host(unsigned long l);
	static unsigned short host2network(short s);
	static short network2host(unsigned short s);
};

#endif	//SOCKET_H