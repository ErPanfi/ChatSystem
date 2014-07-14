#include "DataManager.h"

DataManager::t_usersList DataManager::s_usersList;
DataManager::t_messagesList DataManager::s_messagesList;


void DataManager::userDataReceived(Address senderAddress, User* userData)
{
	t_usersList::iterator iter;
	for(iter = s_usersList.begin(); iter != s_usersList.end() && (**iter).getAddress() == senderAddress; ++iter);

	if((**iter).getAddress() == senderAddress)	//existing user!
	{
		(**iter).refreshFromOther(*userData);	//refresh with received data
		delete userData;						//delete received data
	}
	else	//not found: must create an entry for it
	{
		userData -> setAddress(senderAddress);
		s_usersList.push_front(userData);
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