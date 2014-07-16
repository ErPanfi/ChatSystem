#ifndef MESSAGE_ACK_H
#define MESSAGE_ACK_H

#include "Packable.h"

class MessageAck : public Packable
{
private:
	t_messageNum m_messageNum;
	bool m_isNack;
public:
	
	MessageAck()
		: m_messageNum(0)
		, m_isNack(false)
	{
	}

	MessageAck(t_messageNum num,  bool isNack)
		: m_messageNum(num)
		, m_isNack(isNack)
	{}

	inline t_messageNum getMessageNum() const { return m_messageNum; }
	inline bool isNack() const { return m_isNack; }

	virtual int pack(char buffer[]) const;
	virtual void unpack(char buffer[], int bufSize);
	virtual t_dataType getDataType() const;
};
#endif //MESSAGE_ACK_H