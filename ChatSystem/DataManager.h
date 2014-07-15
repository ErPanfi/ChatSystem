#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "User.h"
#include "Message.h"

#include <list>
#include <string>

class DataManager
{
public:
	typedef std::list<User*> t_usersList;
	typedef std::list<Message*> t_messagesList;

	static const double USER_BCAST_PERIOD;

private:
	static t_usersList s_usersList;
	static t_messagesList s_messagesList;
	static User s_currUser;
	
	static double s_userBcastTimer;

public:
	//nick g/s
	static User* getCurrUser() { return &s_currUser; }	
	static Address getLocalAddress() { return s_currUser.getAddress(); }
	
	//handle received user data
	static void userDataReceived(User* userData);
	static t_usersList::iterator getUserIterator() { return s_usersList.begin();}
	static t_usersList::iterator getUserIteratorEnd() { return s_usersList.end(); }
	//handle received message data
	static void messageReceived(Address senderAddress, Message* messageData);
	static t_messagesList::iterator getMessageIterator() { return s_messagesList.begin();}
	static t_messagesList::iterator getMessageIteratorEnd() { return s_messagesList.end();}
	
	//update method
	static void update(double elapsed);

	//on shutdown
	static void cleanup();
};

#endif