#include "DataManager.h"
#include "Transmitter.h"
#include "PlatformUtils.h"

#include <iostream>
#include <sstream>

DataManager::t_usersList DataManager::s_usersList;
DataManager::t_messagesList DataManager::s_messagesList;
User DataManager::s_currUser;
double DataManager::s_userBcastTimer = 0;

const double DataManager::USER_BCAST_PERIOD = 10.0;

void DataManager::userDataReceived(User* userData)
{
	std::stringstream message;	//for logging 
	Address currUserAddress = s_currUser.getAddress();

	//first: if address is MY local address these are my informations, so we can discard them
	if(currUserAddress == userData -> getAddress())
	{
		message << "My data. Discard them.";
		delete userData;
	}
	else if(currUserAddress.getAddress() || userData -> getNick().compare(s_currUser.getNick()) != 0)	//otherwise if my address is already compiled or this is NOT my nickname
	{
		t_usersList::iterator iter = s_usersList.find(userData);

		if(iter != s_usersList.end())	//existing user!
		{
			message <<  "Fresh data for " << userData->toString();

			(**iter).refreshFromOther(*userData);	//refresh with received data
			delete userData;						//delete received data
		}
		else	//not found: must create an entry for it
		{
			message <<  "New user! " << userData -> toString();
			s_usersList.insert(userData);
			//reply to new user with your informations
			Transmitter::sendDataToAddress(s_currUser, userData->getAddress());
		}
	}
	else	//otherwise this is my host address, which I miss! :D let's save it
	{
		Address completeAddress = s_currUser.getAddress();	//obtain partial data from current user
		completeAddress.setAddress(userData -> getAddress().getAddress());	//manually set IP
		s_currUser.setAddress(completeAddress);
		message << "Obtained local address from bcast : " << s_currUser.getAddress().toString();
		delete userData;
	}

	PlatformUtils::log(message.str());

}

void DataManager::messageReceived(Address senderAddress, Message* messageData)
{
	//TODO add received message to list
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
		PlatformUtils::log("Sending user info on the net...");

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