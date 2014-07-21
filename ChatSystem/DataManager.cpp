#include "DataManager.h"
#include "Transmitter.h"
#include "PlatformUtils.h"
#include "MessageAck.h"
#include "MessageNack.h"

#include <iostream>
#include <sstream>

DataManager::t_usersList DataManager::s_usersList;
DataManager::t_messagesList DataManager::s_messagesList;
User DataManager::s_currUser;
double DataManager::s_userBcastTimer = 0;

const double DataManager::USER_BCAST_PERIOD = 60.0;

void DataManager::userDataReceived(User* userData)
{
	std::stringstream message;	//for logging 
	Address currUserAddress = s_currUser.getAddress();

	//first: if address is MY local address these are my informations, so we can discard them
	if(currUserAddress == userData -> getAddress())
	{
		delete userData;
	}
	else if(currUserAddress.getAddress() || userData -> getNick().compare(s_currUser.getNick()) != 0)	//otherwise if my address is already compiled or this is NOT my nickname
	{
		t_usersList::iterator iter = s_usersList.find(userData);

		if(iter != s_usersList.end())	//existing user!
		{
			(**iter).refreshFromOther(*userData);	//refresh with received data				

			delete userData;						//delete received data

		}
		else	//not found: must create an entry for it
		{
			message <<  "New user detected! " << userData -> toString();
			s_usersList.insert(userData);
			PlatformUtils::log(message.str());
		}

		//reply to sender is handled by transmitter object
	}
	else	//otherwise this is my host address, which I miss! :D let's save it
	{
		Address completeAddress = s_currUser.getAddress();	//obtain partial data from current user
		completeAddress.setAddress(userData -> getAddress().getAddress());	//manually set IP
		s_currUser.setAddress(completeAddress);
		delete userData;
	}
}

void DataManager::messageReceived(Address senderAddress, Message* messageData)
{
	User tmp = User(senderAddress);
	t_usersList::iterator author = s_usersList.find(&tmp);
	
	if(author != s_usersList.end())
	{
		t_messageNum mn = messageData -> getMessageNum();
		messageData -> setAuthor(*author);
		//check if already received message
		t_messagesList::iterator check = s_messagesList.find(messageData);
		if(check == s_messagesList.end())	//not received
		{
			s_messagesList.insert(messageData);
			printMessages();
		}
		else //duplicated message
		{
			delete messageData;
		}

		Transmitter::sendDataToAddress(MessageAck(mn), senderAddress);	//send ack
	}
	else //unknown author
	{
		std::stringstream message;
		message << "Received message from unknown author (" << senderAddress.toString() << ")";
		PlatformUtils::log(message.str());
		Transmitter::sendDataToAddress(MessageNack(), senderAddress);	//send nack
	}
}

void DataManager::messageAckReceived(Address senderAddress, MessageAck* ack)
{
	User tmp = User(senderAddress);
	t_usersList::iterator author = s_usersList.find(&tmp);

	if(author != s_usersList.end())
	{
		(*author) -> ackReceivedForMessage(ack -> getMessageNum());
	}
	else	//unknown author (how is it even possible? :-| )
	{
		std::stringstream message;
		message << "Received message ack from unknown author (" << senderAddress.toString() << ") for message " << ack->getMessageNum();
		PlatformUtils::log(message.str());		
		Transmitter::sendDataToAddress(MessageNack(), senderAddress);	//resend me your data, please
	}
}

void DataManager::cleanup()
{
	for(t_usersList::iterator uIter = s_usersList.begin(); uIter != s_usersList.end(); ++ uIter)
	{
		delete (*uIter);
	}

	for(t_messagesList::iterator mIter = s_messagesList.begin(); mIter != s_messagesList.end(); ++mIter)
	{
		delete (*mIter);
	}
}

void DataManager::update(double elapsed)
{
	if((s_userBcastTimer -= elapsed) <= 0)
	{
		//PlatformUtils::log("Sending user info on the net...");

		s_userBcastTimer = USER_BCAST_PERIOD;

		Transmitter::sendBcastData(s_currUser);
	}
}

void DataManager::printUsers()
{
	std::cout << "Connected users: " << std::endl << s_currUser.toString() << std::endl;
	for(t_usersList::iterator iter = s_usersList.begin(); iter != s_usersList.end(); ++iter)
	{
		std::cout << (*iter) -> toString() << std::endl;
	}
}

const unsigned short DataManager::MAX_MESSAGE_PRINT = 8;

void DataManager::printMessages(unsigned short howMany)
{
	int i;
	t_messagesList::iterator mIter;
	std::stringstream message;
	std::string output = "";
	for(i = 0, mIter = s_messagesList.begin(); i < howMany && mIter != s_messagesList.end(); ++i, ++mIter)
	{
		message.str("");
		message << "(" << PlatformUtils::relativeTime2str((*mIter) -> getSendingTime()) << ") " //
				<< (*mIter) -> getAuthor() -> getNick() << " : " //
				<< (*mIter) -> getMessage() << std::endl;
		output = message.str().append(output);
	}

	std::cout	<< std::endl << std::endl << std::endl << std::endl << std::endl //
				<< std::endl << "*************************************" << std::endl << std::endl << output //
				<< std::endl << "*************************************" << std::endl;
}

void DataManager::writeNewMessage()
{
	Message* newMessage = new Message(++s_currUser.m_maxMessageAcked);
	std::string text;
	std::cout << "Message text (max " << Message::MAX_MESSAGE_LEN << " chars): ";
	getline(std::cin, text);
	if(!text.empty())
	{
		newMessage -> setMessage(text.substr(0, Message::MAX_MESSAGE_LEN));
		newMessage -> setAuthor(&s_currUser);
		s_messagesList.insert(newMessage);
		Transmitter::sendDataToPeers(*newMessage);
	}
	else	//don't send empty messages
	{
		delete newMessage;
		--s_currUser.m_maxMessageAcked;
	}
}

std::list<User*> DataManager::missingRecipients(Message* msg)
{
	std::list<User*> ret;

	for(t_usersList::iterator uIter = s_usersList.begin(); uIter != s_usersList.end(); ++uIter)
	{
		if((*uIter) -> shouldResendMessage(msg -> getMessageNum()))
		{
			ret.push_back(*uIter);
		}
	}

	return ret;
}