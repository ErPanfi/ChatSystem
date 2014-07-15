#ifndef USER_H
#define USER_H

#include "Message.h"
#include "Address.h"

#include <string>
#include <iostream>
#include <list>

class User : public Packable
{
public:
	static const unsigned short MAX_NICK_LEN = 51;
	typedef std::string t_nickType;

private:
	Address m_address;
	t_nickType m_nick;
	
	std::list<Message *> messageList;

public:

	User() {}
	User(Address address) : m_address(address) {}

	Address getAddress() const { return m_address; }
	void setAddress(Address address) { m_address = address;}

	t_nickType getNick() const { return m_nick;}
	void setNick(t_nickType newNick) { m_nick = newNick;}

	virtual int pack(char buffer[]) const;
	virtual void unpack(char buffer[], int bufSize);
	
	virtual t_dataType getDataType() const { return t_dataType::UserType;}

	void refreshFromOther(User &other);

	std::string toString() const;
};

struct UserComparator
{
	bool operator()(const User* const lhs , const User* const rhs) const { return lhs -> getAddress() < rhs -> getAddress(); }
};



#endif