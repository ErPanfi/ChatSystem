#include "Address.h"

Address::Address()
{
	init(0,0);
}

Address::Address(t_address address, t_port port)
{
	init(address, port);
}

Address::Address(	t_octet	o1,
					t_octet	o2,
					t_octet	o3, 
					t_octet	o4,
					t_port	port
				)
{
	init(o1, o2, o3, o4, port);
}

void Address::init(t_address address, t_port port)
{
	m_address = address;
	m_port = port;
}

void Address::init(t_octet o1, t_octet o2, t_octet o3, t_octet o4, t_port port)
{
	m_port = port;
	m_address = (t_address)(o1 << 24 | o2 << 16 | o3 << 8 | o4);
}