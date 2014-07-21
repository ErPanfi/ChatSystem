#ifndef MESSAGE_NACK_H
#define MESSAGE_NACK_H

#include "Packable.h"

//this class represent a nack in response to a message (typically sent when a formally correct message is received from an unrecognized IP address)
class MessageNack : public Packable
{
public:
	
	virtual t_dataType getDataType() const { return t_dataType::MessageNack; }
};

#endif //MESSAGE_NACK_H