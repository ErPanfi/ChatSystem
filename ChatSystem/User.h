#ifndef USER_H
#define USER_H

#include "Packable.h"
#include "Address.h"

#include <string>
#include <iostream>
#include <list>

class User : public Packable
{
public:
	static const unsigned short MAX_NICK_LEN = 51;
	typedef std::string t_nickType;

	typedef unsigned short t_ackMask;

	friend class DataManager;

private:
	Address m_address;
	t_nickType m_nick;
	t_messageNum m_maxMessageAcked;		//this in current user is the current message sent
	t_ackMask m_ackMask;				//unused in current user

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
	void ackReceivedForMessage(unsigned short acked);

	virtual int pack(char buffer[]) const;
	virtual void unpack(char buffer[], int bufSize);
	
	virtual t_dataType getDataType() const { return t_dataType::UserData;}

	void refreshFromOther(User &other);

	std::string toString() const;
};

struct UserComparator
{
	inline bool operator()(const User* const lhs , const User* const rhs) const { return lhs -> getAddress() < rhs -> getAddress(); }
};



#endif //USER_H