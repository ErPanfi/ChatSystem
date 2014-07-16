#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include "User.h"
#include "Message.h"

#include <list>
#include <set>
#include <string>

class DataManager
{
public:
	typedef std::set<User*, struct UserComparator> t_usersList;
	typedef std::set<Message*, struct MessageComparator> t_messagesList;

	static const double USER_BCAST_PERIOD;

private:
	static t_usersList s_usersList;
	static t_messagesList s_messagesList;

	static User s_currUser;	
	static double s_userBcastTimer;
	static unsigned short s_maxMessageSent;

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
	
	//write new message
	static void writeNewMessage();

	//chat data visualization methods
	static void printUsers();
	static const unsigned short MAX_MESSAGE_PRINT;
	static void printMessages(unsigned short howMany = MAX_MESSAGE_PRINT);

	//update method
	static void update(double elapsed);

	//on init and shutdown
	static void cleanup();
};

#endif //DATA_MANAGER_H