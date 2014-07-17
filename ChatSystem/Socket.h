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

	bool open(t_port port);
	void close();
	bool isOpen() const;
	bool send(const Address& receiver, const void* data, int size);
	int receive(Address& sender, void* data, int size);
	
	bool initializeSocketSystem();
	void cleanupSocketSystem();

	static unsigned long host2network(long l);
	static long network2host(unsigned long l);
	static unsigned short host2network(short s);
	static short network2host(unsigned short s);

	/*
	unsigned short packInBuffer(char c, char buffer[]);
	unsigned short packInBuffer(double d, char buffer[]);
	unsigned short packInBuffer(time_t t, char buffer[]);
	unsigned short packInBuffer(long i, char buffer[]);
	unsigned short packInBuffer(short s, char buffer[]);

	char unpackCharFromBuffer(char* buf);
	double unpackDoubleFromBuffer(char* buf);
	time_t unpackTimeFromBuffer(char* buf);
	long unpackLongFromBuffer(char* buf);
	short unpackShortFromBuffer(char* buf);
	*/
};

#endif	//SOCKET_H now defined