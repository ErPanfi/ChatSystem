#ifndef USER_H
#define USER_H

#include "Message.h"
#include "Address.h"

#include <string>
#include <list>

class User : public Packable
{
public:
	static const unsigned short MAX_NICK_LEN = 50;

private:
	Address m_address;
	char m_nick[MAX_NICK_LEN];
	
	std::list<Message *> messageList;

public:
	Address getAddress() const { return m_address; }
	void setAddress(Address address) { m_address = address;}

	std::string getNick() const { return std::string(m_nick);}	

	virtual int pack(char buffer[]) const;
	virtual void unpack(char buffer[], int bufSize);
	
	virtual t_dataType getDataType() const { return t_dataType::UserType;}

	void refreshFromOther(User &other);

};

#endif