#include "Address.h"

#include <sstream>

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

std::string Address::toString() const
{
	std::stringstream ret;

	ret << getO1() << "." << getO2() << "." << getO3() << "." << getO4() << ":" << m_port;

	return ret.str();
}