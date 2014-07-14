#include "Address.h"

#include <stdlib.h>

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

std::string Address::getAddressStr() const
{
	std::string ret;
	ret += getO1();
	ret += '.';
	ret += getO2();
	ret += '.';
	ret += getO3();
	ret += '.';
	ret += getO4();
	ret += ':';
	ret += m_port;


	return ret;
}