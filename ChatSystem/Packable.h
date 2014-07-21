#ifndef PACKABLE_H
#define PACKABLE_H

//This class is at root of everything that can be sent through the network
class Packable
{
public:

	//Every type of data that can be sent 
	typedef enum DataTypeEnum
	{
		MessageData = 0,
		UserData,
		MessageAck,
		UserAck,
		MessageNack,	//this is used when a message arrives from an unknown author (shouldn't happen)
		COUNT
	} t_dataType;

	//hardcoded packet size. TODO: ask NIC for max packet size
	static const unsigned short MAX_PACKET_SIZE = 256;	
	//Number of preamble bytes for each packet
	static const unsigned short CHAT_PROTO_NBYTES = 4;	
	//the preamble bytes in question
	static const char CHAT_PROTO_ID[CHAT_PROTO_NBYTES];	

	//this is used to put into buffer a byte-sequenced representation of the data. Assumes enough memory space is available in buffer.
	virtual int pack(char buffer[]) const;				
	//when called on a buffer filled by pack() function this should fill in every field that has been serialized into buffer
	virtual void unpack(char buffer[], int bufSize) {}
	
	//this is used to obtain what data type a packed class contains (useful for 
	virtual t_dataType getDataType() const = 0;
	
	//this is used to validate a compiled buffer: preamble bytes are checked and packet type is extracted, casted to (int) and returned. If sommething don't match a -1 is returned, instead.
	static int validatePackage(char buffer[], int bufSize);
};

//some types which are common to all packable classes are declared here
typedef short t_messageNum;


#endif //PACKABLE_H