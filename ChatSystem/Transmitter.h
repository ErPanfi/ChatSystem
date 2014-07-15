#ifndef CHAT_SYSTEM_H
#define CHAT_SYSTEM_H

#include "Packable.h"
#include "Socket.h"

#include <string>


class Transmitter
{

private:
	static Socket s_socket;
	static Socket::t_port s_port;

public:	
	static bool initTransmitter(Socket::t_port port);
	static bool sendDataToPeers(Packable &data);
	static bool sendBcastData(Packable &data);
	static bool sendDataToAddress(Packable &data, Address address);
	static Packable* receiveData();
};


#endif