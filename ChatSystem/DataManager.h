#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "User.h"
#include "Message.h"

#include <list>

class DataManager
{
public:
	typedef std::list<User*> t_usersList;
	typedef std::list<Message*> t_messagesList;

private:
	static t_usersList s_usersList;
	static t_messagesList s_messagesList;

public:
	static void userDataReceived(Address senderAddress, User* userData);
	static t_usersList::iterator getUserIterator() { return s_usersList.begin();}

	static void messageReceived(Address senderAddress, Message* messageData);
	static t_messagesList::iterator getMessageIterator() { return s_messagesList.begin();}

	static void cleanup();
};

#endif