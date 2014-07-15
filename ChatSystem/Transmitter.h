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
	//initialization step
	static bool initTransmitter(Socket::t_port port);
	//sending primitives
	static bool sendDataToPeers(Packable &data);	//to connected peers
	static bool sendBcastData(Packable &data);		//to everyone on LAN
	static bool sendDataToAddress(Packable &data, Address address);	//to specific address
	//receive data
	static void receiveData();
	//update method
	static void update(double elapsed);
};


#endif