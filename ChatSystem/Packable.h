#ifndef PACKABLE_H
#define PACKABLE_H

class Packable
{
public:

	typedef enum DataTypeEnum
	{
		MessageData = 0,
		UserData,
		MessageAck,
		UserAck,
		MessageNack,	//this is used when a message arrives from an unknown author (shouldn't happen)
	} t_dataType;

	static const unsigned short MAX_PACKET_SIZE = 256;
	static const unsigned short CHAT_PROTO_NBYTES = 4;
	static const char CHAT_PROTO_ID[CHAT_PROTO_NBYTES];

	virtual int pack(char buffer[]) const;
	virtual void unpack(char buffer[], int bufSize) = 0;
	
	virtual t_dataType getDataType() const = 0;
	
	static int validatePackage(char buffer[], int bufSize);
};

//some types which are common to all packable classes are declared here
typedef unsigned short t_messageNum;


#endif