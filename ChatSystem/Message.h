#ifndef MESSAGE_H
#define MESSAGE_H

#include "PlatformUtils.h"
#include "Packable.h"
#include "User.h"
#include <string>

//This class wraps the message that is sent through the network
class Message : public Packable
{
public:
	//maximum message length. TODO: support unlimited message length with message split
	static const unsigned short MAX_MESSAGE_LEN = 140;

	typedef std::string t_message;

private:

	User *m_author;
	t_message m_message;
	//this is used to reorder messages when merged in a single list
	PlatformUtils::t_relativeTime m_sendingTime;
	//this is used to reorder the messages from the same author and to ack them correctly
	t_messageNum m_messageNum;

public:

	Message()
		: m_sendingTime(0)
		, m_messageNum(0)
	{
	}

	Message(t_messageNum num)
		: m_sendingTime(PlatformUtils::time2relative())
		, m_messageNum(num)
	{
	}

	inline PlatformUtils::t_relativeTime getSendingTime() const { return m_sendingTime; }
	inline void setSendingTime(PlatformUtils::t_relativeTime newTime = PlatformUtils::time2relative()) { m_sendingTime = newTime; }	

	inline t_message getMessage() const { return m_message;}
	inline void setMessage(t_message newMessage) { m_message = newMessage; }

	inline User* getAuthor() const { return m_author; }
	inline void setAuthor(User* auth) { m_author = auth; }

	inline t_messageNum getMessageNum() const { return m_messageNum; }

	virtual int pack(char buffer[]) const;
	virtual void unpack(char buffer[], int bufSize);
	virtual t_dataType getDataType() const { return t_dataType::MessageData;}

	//this is useful when using std:: sorted data structures
	inline bool operator< (const Message &other) const		
	{ 
		return (
					m_sendingTime > other.m_sendingTime ||	//the lesser message is the recentmost one
					(
						m_sendingTime == other.m_sendingTime && 
						struct UserComparator()(m_author, other.m_author)
					)
				);
	}
};

//an external trivial comparator that can be fed on a third party sorted structure
struct MessageComparator
{
	inline bool operator()(const Message* const &lhs, const Message* const &rhs) { return (*lhs) < (*rhs); }
};

#endif //MESSAGE_H