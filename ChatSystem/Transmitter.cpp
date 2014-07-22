#include "Transmitter.h"
#include "DataManager.h"
#include "PlatformUtils.h"

#include <iostream>
#include <sstream>

Socket Transmitter::s_socket;

bool Transmitter::initTransmitter(Socket::t_port port)
{
	if(s_socket.initializeSocketSystem() && s_socket.open(port))
	{
		DataManager::getCurrUser() -> setAddress(Address(0, port));
		return true;
	}
	else
	{
		return false;
	}
}

bool Transmitter::sendDataToPeers(Packable &data)
{
	DataManager::t_usersList::iterator uIter = DataManager::getUserIterator();
	if(uIter != DataManager::getUserIteratorEnd())	//skip everyithing if peers list is empty
	{
		//pack data only once
		char packetData[Packable::MAX_PACKET_SIZE];
		memset(packetData, 0, Packable::MAX_PACKET_SIZE);
		int packetSize = data.pack(packetData);

		for(; uIter != DataManager::getUserIteratorEnd(); ++uIter)
		{

#ifdef MESSAGE_CONDITIONAL_SENDING	//this is for testing message redending  and message reordering
			std::cout << "Send " << data.getDataType() << "  to " << (*uIter) -> getAddress().toString() << "? (Press 'S' to skip, other key to send)" << std::endl;
			switch (getchar())
			{
			case 's':
			case 'S':
				//do nothing
				break;
			default:
			s_socket.send( (*uIter) -> getAddress(), packetData, packetSize);
				break;
			}
#else
			s_socket.send( (*uIter) -> getAddress(), packetData, packetSize);

#endif // MESSAGE_CONDITIONAL_SENDING

		}

	}

	//add messages to resend list
	if(data.getDataType() == Packable::t_dataType::MessageData)
	{
		t_resendDescriptor desc;
		desc.message = (Message*) &data;
		s_resendList.push_back(desc);
	}

	return true;
}

//used for acks and message resending
bool Transmitter::sendDataToAddress(Packable &data, Address address)	
{
	char packetData[Packable::MAX_PACKET_SIZE];
	memset(packetData, 0, Packable::MAX_PACKET_SIZE);
	int packetSize = data.pack(packetData);

#ifdef MESSAGE_CONDITIONAL_SENDING	//this is for testing message redending  and message reordering
			std::cout << "Send " << data.getDataType() << "  to " << address.toString() << "? (Press 'S' to skip, other key to send)" << std::endl;
			switch (getchar())
			{
			case 's':
			case 'S':
				return true;
				break;
			}
#endif // MESSAGE_CONDITIONAL_SENDING

	return s_socket.send( address, packetData, packetSize);
}

//used for user acks
bool Transmitter::sendDataToAddress(Packable &data, Address address, Packable::t_dataType dataType)
{
	char packetData[Packable::MAX_PACKET_SIZE];
	memset(packetData, 0, Packable::MAX_PACKET_SIZE);
	int packetSize = data.pack(packetData);
	packetData[Packable::CHAT_PROTO_NBYTES] = (char) dataType;	//overwrite type byte

	return s_socket.send(address, packetData, packetSize);
}

//used for broadcasting self username
bool Transmitter::sendBcastData(Packable &data)
{
	Address address;
	address.init(255,255,255,255, DataManager::getCurrUser() -> getAddress().getPort());	//using global BCast address: routers don't forward these packets to the outer world
	char packetData[Packable::MAX_PACKET_SIZE];
	memset(packetData, 0, Packable::MAX_PACKET_SIZE);
	int packetSize = data.pack(packetData);

	return s_socket.send(address, packetData, packetSize);
}

void Transmitter::update(double elapsed)
{
	//each frame check for pending data to be received
	receiveData();
	//for each pending resend check if should be performed
	tickAndResend(elapsed);
}

void Transmitter::receiveData()
{
	Address senderAddress;
	char packetData[Packable::MAX_PACKET_SIZE];
	int res;
	Message *messageData = NULL;
	User *userData = NULL;
	MessageAck* messageAck = NULL;

	std::stringstream message;

	//if(s_socket.receive(senderAddress, &packetPayload, sizeof(t_packetPayload))>0)
	int packetSize = s_socket.receive(senderAddress, packetData, Packable::MAX_PACKET_SIZE);
	if(packetSize > 0 && (res = Packable::validatePackage(packetData, packetSize)) >= 0)
	{
		//received valid packet... but which type?
		switch (res)
		{
		case Packable::t_dataType::MessageData:	//a message! unpack it and forward it do data manager
			messageData = new Message();
			messageData  -> unpack(packetData, packetSize);
			DataManager::messageReceived(senderAddress, messageData);
			break;

		case Packable::t_dataType::UserData:	//if this has been received the sender is waiting for an ack. Send him.
			sendDataToAddress(*DataManager::getCurrUser(), senderAddress, Packable::t_dataType::UserAck);
		case Packable::t_dataType::UserAck:		//otherwise what we received is an ack to our request, so we can simply process it
			userData = new User(senderAddress);
			userData -> unpack(packetData, packetSize);
			DataManager::userDataReceived(userData);			
			break;

		case Packable::t_dataType::MessageNack:	//sender doesn't know me: send my data to him as an ack to his request
			sendDataToAddress(*DataManager::getCurrUser(), senderAddress, Packable::t_dataType::UserAck);
			break;
		case Packable::t_dataType::MessageAck:	//I received an ack to a message! store it in sender ackmask
			messageAck = new MessageAck();
			messageAck -> unpack(packetData, packetSize);
			DataManager::messageAckReceived(senderAddress, messageAck);
			break;
		default:	//this packet has not been recognized
			
			message << "Unrecognized packet from " << senderAddress.toString() ;
			PlatformUtils::log(message.str());
			break;
		}
		
	}
}

Transmitter::t_resendDescriptorList Transmitter::s_resendList;
const double Transmitter::RESEND_TIMESPAN_SECONDS = 0.75;

void Transmitter::tickAndResend(double elapsed)
{
	t_resendDescriptorList::iterator rIter = s_resendList.begin(); 
	while(rIter != s_resendList.end())
	{
		rIter -> resendTimer -= elapsed;
		if( (rIter -> resendTimer) <= 0)	//it's time for a resend!
		{
			//obtain list of user which did not acked the message
			std::list<User*> recipients = DataManager::missingRecipients(rIter -> message);

			if(!recipients.empty())	
			{
				//resend the message to each user
				for(std::list<User*>::iterator uIter = recipients.begin(); uIter != recipients.end(); ++uIter)
				{
					Transmitter::sendDataToAddress(*(rIter -> message), (*uIter) -> getAddress());
				}

				rIter -> resendTimer = RESEND_TIMESPAN_SECONDS;	//restore timer
				++rIter;	//next resend
			}
			else	//everyone received message!
			{
				s_resendList.erase(rIter++);	//remove resend descriptor, because no resends are necessary for this message
			}
		}
		else
		{
			++rIter;	//nothing to do: next resend descriptor, please
		}
	}
}

void Transmitter::cleanup()
{
	s_socket.close();
	s_socket.cleanupSocketSystem();
}