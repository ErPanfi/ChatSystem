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

void Address::getAddressStr(t_addressStr address) const
{
	int i = 0;
	itoa(getO1(), address, 10);
	for(; address[i]!='\0';++i);	//skip to null char
	address[i++] = '.';
	itoa(getO2(),&(address[i]),10);
	for(; address[i]!='\0';++i);	//skip to null char
	address[i++] = '.';
	itoa(getO3(),&(address[i]),10);
	for(; address[i]!='\0';++i);	//skip to null char
	address[i++] = '.';
	itoa(getO4(),&(address[i]),10);
	for(; address[i]!='\0';++i);	//skip to null char
	address[i++] = ':';
	itoa(m_port,&(address[i]),10);
}