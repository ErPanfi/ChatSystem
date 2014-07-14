#ifndef MESSAGE_H
#define MESSAGE_H

#include "Packable.h"
#include <string>

class Message : public Packable
{
public:
	static const unsigned short MAX_MESSAGE_LEN = 141;

private:
	char m_message[MAX_MESSAGE_LEN];

public:

	std::string getMessage() const { return std::string(m_message);}

	virtual int pack(char buffer[]) const;
	virtual void unpack(char buffer[], int bufSize);
	virtual t_dataType getDataType() const { return t_dataType::MessageType;}

};

#endif