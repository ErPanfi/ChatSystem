#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include "User.h"
#include "Message.h"
#include "MessageAck.h"

#include <list>
#include <set>
#include <string>

class DataManager
{
public:
	//this will make the user list always sorted, and grant O(log n) search operations
	typedef std::set<User*, struct UserComparator> t_usersList;

	//this will make the user list always sorted, and grant O(log n) search operations.
	typedef std::set<Message*, struct MessageComparator> t_messagesList;

	static const double USER_BCAST_PERIOD;

private:
	//all the recognized users. TODO: user disconnection should be detected with keepalive packets
	static t_usersList s_usersList;	
	//all the saved messages
	static t_messagesList s_messagesList;

	//the host user
	static User s_currUser;	
	//this countdown is used to broadcast my data every USER_BCAST_PERIOD seconds
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
	static void messageAckReceived(Address senderAddress, MessageAck* ack);
	static t_messagesList::iterator getMessageIterator() { return s_messagesList.begin();}
	static t_messagesList::iterator getMessageIteratorEnd() { return s_messagesList.end();}
	
	//write new message
	static void writeNewMessage();

	//obtain peers at which given message should be resended
	static std::list<User*> missingRecipients(Message* msg);

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