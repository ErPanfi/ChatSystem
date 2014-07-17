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
		char packetData[Packable::MAX_PACKET_SIZE];
		memset(packetData, 0, Packable::MAX_PACKET_SIZE);
		int packetSize = data.pack(packetData);

		for(; uIter != DataManager::getUserIteratorEnd(); ++uIter)
		{
			s_socket.send( (*uIter) -> getAddress(), packetData, packetSize);
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

bool Transmitter::sendDataToAddress(Packable &data, Address address)
{
	char packetData[Packable::MAX_PACKET_SIZE];
	memset(packetData, 0, Packable::MAX_PACKET_SIZE);
	int packetSize = data.pack(packetData);

	return s_socket.send(address, packetData, packetSize);
}

bool Transmitter::sendDataToAddress(Packable &data, Address address, Packable::t_dataType dataType)
{
	char packetData[Packable::MAX_PACKET_SIZE];
	memset(packetData, 0, Packable::MAX_PACKET_SIZE);
	int packetSize = data.pack(packetData);
	packetData[Packable::CHAT_PROTO_NBYTES] = (char) dataType;	//overwrite type byte

	return s_socket.send(address, packetData, packetSize);
}

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
	receiveData();
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
		switch (res)
		{
		case Packable::t_dataType::MessageData:
			messageData = new Message();
			messageData  -> unpack(packetData, packetSize);
			DataManager::messageReceived(senderAddress, messageData);
			break;

		case Packable::t_dataType::UserData:	//if this has been received the sender is waiting for an ack. Send him.
			sendDataToAddress(*DataManager::getCurrUser(), senderAddress, Packable::t_dataType::UserAck);
		case Packable::t_dataType::UserAck:		//otherwise what we received is already an ack to our request
			userData = new User(senderAddress);
			userData -> unpack(packetData, packetSize);
			DataManager::userDataReceived(userData);			
			break;

		case Packable::t_dataType::MessageNack:	//sender doesn't know me: send my data
			sendDataToAddress(*DataManager::getCurrUser(), senderAddress, Packable::t_dataType::UserAck);
			break;
		case Packable::t_dataType::MessageAck:
			messageAck = new MessageAck();
			messageAck -> unpack(packetData, packetSize);
			DataManager::messageAckReceived(senderAddress, messageAck);
			break;
		default:
			
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
		if( (rIter -> resendTimer) <= 0)
		{
			std::list<User*> recipients = DataManager::missingRecipients(rIter -> message);

			if(!recipients.empty())
			{
				for(std::list<User*>::iterator uIter = recipients.begin(); uIter != recipients.end(); ++uIter)
				{
					Transmitter::sendDataToAddress(*(rIter -> message), (*uIter) -> getAddress());
				}

				rIter -> resendTimer = RESEND_TIMESPAN_SECONDS;	//restore timer
				++rIter;	//next resend
			}
			else
			{
				s_resendList.erase(rIter++);	//every recipient is satisfied: remove resend descriptor
			}
		}
		else
		{
			++rIter;	//nothing to do: next resend descriptor, please
		}
	}
}