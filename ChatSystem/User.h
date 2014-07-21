#ifndef USER_H
#define USER_H

#include "Packable.h"
#include "Address.h"

#include <string>
#include <iostream>
#include <list>

//This class wraps the user data that peers exchange to identify themselves.
class User : public Packable
{
public:
	static const unsigned short MAX_NICK_LEN = 51;
	typedef std::string t_nickType;

	//ack bitmask store the status of 8*sizeof ack preceding the recentmost one: corresponding bit is 1 iff ack has been received for that message
	typedef unsigned short t_ackMask;

	//see DataManager::WriteNewMessage() function. Imho this is more correct, logically speaking, than two dedicated inlined accessors.
	friend class DataManager;

private:
	//the user IP address
	Address m_address;
	//the user nickname
	t_nickType m_nick;
	//In any peer this is the maximum ack number peer have sent; in current user is the maximum message number sent
	t_messageNum m_maxMessageAcked;		
	//the received ack bitmask (unused in current user)
	t_ackMask m_ackMask;				

public:

	User()
		: m_nick(std::string(""))
		, m_maxMessageAcked(0)
		, m_ackMask(0)
	{}

	User(Address address) 
		: m_address(address) 
		, m_nick(std::string(""))
		, m_maxMessageAcked(0)
		, m_ackMask(0)
	{
	}


	inline Address getAddress() const { return m_address; }
	inline void setAddress(Address address) { m_address = address;}

	inline t_nickType getNick() const { return m_nick;}
	inline void setNick(t_nickType newNick) { m_nick = newNick;}

	inline unsigned short getMaxMessageSent() const { return m_maxMessageAcked; }
	inline void setMaxMessageSent(unsigned short newMax) { m_maxMessageAcked = newMax; }

	//this is called when an ack is received for the given message from this user
	void ackReceivedForMessage(t_messageNum acked);

	/**
	 * Packable functions
	 */

	virtual int pack(char buffer[]) const;
	virtual void unpack(char buffer[], int bufSize);
	
	virtual t_dataType getDataType() const { return t_dataType::UserData;}

	//this is used to refresh some class members with data received from the network (can be used when a nickname changes)
	void refreshFromOther(User &other);

	//this returns true when the given message should be resent to this user
	inline bool shouldResendMessage(t_messageNum messageNum) const
	{
		return messageNum > (m_maxMessageAcked - 1 - (short)(8 * sizeof(t_ackMask)))	//message is not too old
			&& messageNum != m_maxMessageAcked											//message is not already acked (because it's max acked message)
			&& (	 messageNum > m_maxMessageAcked										//message is younger that max message acked
				||	!(m_ackMask & (1 << (m_maxMessageAcked - messageNum - 1)))			//in ackMask there's a zero corresponding to this message
				);
	}

	std::string toString() const;
};

struct UserComparator
{
	inline bool operator()(const User* const lhs , const User* const rhs) const { return lhs -> getAddress() < rhs -> getAddress(); }
};



#endif //USER_H