#ifndef ADDRESS_H
#define ADDRESS_H

class Address
{
public:
	typedef unsigned int t_address;
	typedef unsigned short t_port;
	typedef unsigned char t_octet;

private:
	t_address m_address;
	t_port m_port;


public:
	
	Address(t_address address, t_port port);
	Address();
	Address(	t_octet o1,
				t_octet o2,
				t_octet o3, 
				t_octet o4,
				t_port port
				);

	//these are trivial getters, can be inlined
	inline t_address getAddress() const	{ return m_address; }
	inline t_octet getO1() const		{ return (t_octet) (m_address >> 24); }
	inline t_octet getO2() const		{ return (t_octet) (m_address >> 16); }
	inline t_octet getO3() const		{ return (t_octet) (m_address >> 8); }
	inline t_octet getO4() const		{ return (t_octet) (m_address); }
	inline t_port getPort() const		{ return m_port; }

	void init(t_address address, t_port port);
	void init(t_octet o1, t_octet o2, t_octet o3, t_octet o4, t_port port);
};

#endif