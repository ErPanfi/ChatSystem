#ifndef MESSAGE_NACK_H
#define MESSAGE_NACK_H

#include "Packable.h"

class MessageNack : public Packable
{
public:
	
	virtual t_dataType getDataType() const { return t_dataType::MessageNack; }
};

#endif //MESSAGE_NACK_H