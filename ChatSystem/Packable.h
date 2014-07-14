#ifndef PACKABLE_H
#define PACKABLE_H

class Packable
{
public:

	typedef enum DataTypeEnum
	{
		MessageType = 0,
		UserType,
	} t_dataType;

	static const unsigned short MAX_PACKET_SIZE = 256;
	static const unsigned short CHAT_PROTO_NBYTES = 4;
	static const char CHAT_PROTO_ID[CHAT_PROTO_NBYTES];

	virtual int pack(char buffer[]) const = 0;
	virtual void unpack(char buffer[], int bufSize) = 0;
	
	virtual t_dataType getDataType() const = 0;
	
	static int validatePackage(char buffer[], int bufSize);
};

#endif