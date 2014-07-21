#ifndef ADDRESS_H
#define ADDRESS_H

#include <string>

//This class wraps IP address. Theoretically this should guarantee ip-version-agnosticness, but as today it only supports ipv4
class Address
{
public:
	typedef unsigned int t_address;
	typedef unsigned short t_port;
	typedef unsigned char t_octet;

private:
	t_address m_address;
	t_port m_port;

	inline unsigned short getO1() const	{ return (t_octet) (m_address >> 24); }
	inline unsigned short getO2() const	{ return (t_octet) (m_address >> 16); }
	inline unsigned short getO3() const	{ return (t_octet) (m_address >> 8); }
	inline unsigned short getO4() const	{ return (t_octet) (m_address); }


public:

	Address()
		: m_address(0)
		, m_port(0)
	{
	}

	Address(t_address addr, t_port port)
		: m_address(addr)
		, m_port(port)
	{
	}
	
	//these are trivial getters, can be inlined
	inline t_address getAddress() const	{ return m_address; }
	inline void setAddress(t_address addr) { m_address = addr; }
	inline t_port getPort() const		{ return m_port; }
	inline void setPort(t_port newPort) { m_port = newPort; }
	std::string toString() const;

	void init(t_address address, t_port port);
	void init(t_octet o1, t_octet o2, t_octet o3, t_octet o4, t_port port);

	bool operator== (const Address &other) const { return m_address == other.m_address && m_port == other.m_port; }
	bool operator< (const Address &other) const { return m_address < other.m_address; }

};

#endif //ADDRESS_H