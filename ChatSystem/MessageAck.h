#ifndef MESSAGE_ACK_H
#define MESSAGE_ACK_H

#include "Packable.h"

//This is sent in response to a correctly received message
class MessageAck : public Packable
{
private:
	t_messageNum m_messageNum;
public:
	
	MessageAck()
		: m_messageNum(0)
	{
	}

	MessageAck(t_messageNum num)
		: m_messageNum(num)
	{}

	inline t_messageNum getMessageNum() const { return m_messageNum; }

	virtual int pack(char buffer[]) const;
	virtual void unpack(char buffer[], int bufSize);
	virtual t_dataType getDataType() const;
};
#endif //MESSAGE_ACK_H