#include "DataManager.h"
#include "Transmitter.h"
#include "PlatformUtils.h"

DataManager::t_usersList DataManager::s_usersList;
DataManager::t_messagesList DataManager::s_messagesList;
User DataManager::s_currUser;
double DataManager::s_userBcastTimer = 0;

const double DataManager::USER_BCAST_PERIOD = 120.0;

void DataManager::userDataReceived(User* userData)
{
	t_usersList::iterator iter;
	Address senderAddress = userData -> getAddress();	//prevent continuous method calling in iterations
	for(iter = s_usersList.begin(); iter != s_usersList.end() && (**iter).getAddress() == senderAddress; ++iter);	//

	if(iter != s_usersList.end())	//existing user!
	{
		PlatformUtils::log("Fresh data for "+userData->toString());
		(**iter).refreshFromOther(*userData);	//refresh with received data
		delete userData;						//delete received data
	}
	else	//not found: must create an entry for it
	{
		PlatformUtils::log("New user! "+userData -> toString());
		s_usersList.push_front(userData);
		//reply to new user with your informations
		Transmitter::sendDataToAddress(s_currUser, userData->getAddress());
	}
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