#ifndef CHAT_SYSTEM_H
#define CHAT_SYSTEM_H

#include "Packable.h"
#include "Socket.h"

#include <string>


class Transmitter
{

private:
	static Socket s_socket;

public:	
	static bool initTransmitter(unsigned short port);
	static bool sendData(Packable &data);
	static Packable* receiveData();
};


#endif