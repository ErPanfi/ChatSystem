#ifndef TRANSMITTER_H
#define TRANSMITTER_H

//#define MESSAGE_CONDITIONAL_SENDING		//this is for packet loss debug

#include "Packable.h"
#include "Socket.h"

#include "Message.h"
#include <string>
#include <list>


//This is used to transmit and receive data through the network
class Transmitter
{
public:

	static const double RESEND_TIMESPAN_SECONDS;

	//this describe a single resend operation, to be performed after a given amount of seconds
	typedef struct ResendDescriptor
	{
		Message* message;
		double resendTimer;

		ResendDescriptor()
			: message(NULL)
			, resendTimer(RESEND_TIMESPAN_SECONDS)
		{}
	} t_resendDescriptor;

	typedef std::list<t_resendDescriptor> t_resendDescriptorList;

private:
	static Socket s_socket;

	//service method, allow to force a certain data type on a packet
	static bool sendDataToAddress(Packable &data, Address address, Packable::t_dataType dataType);
	//a list of resends that still have to be performed
	static t_resendDescriptorList s_resendList;
	//check for resends to do this frame
	static void tickAndResend(double elapsed);

	//receive data
	static void receiveData();



public:	
	//initialization step
	static bool initTransmitter(Socket::t_port port);

	//sending primitives
	static bool sendDataToPeers(Packable &data);	//to connected peers
	static bool sendBcastData(Packable &data);		//to everyone on LAN
	static bool sendDataToAddress(Packable &data, Address address);	//to specific address

	//update method
	static void update(double elapsed);

	//socket closure method
	static void cleanup();
};


#endif //TRANSMITTER_H