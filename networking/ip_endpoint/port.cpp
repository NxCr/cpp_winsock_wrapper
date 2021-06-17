#pragma once
#include "port.h"
#include "../lib_includes.cpp"

unsigned short Autumn::networking::Port::to_network(unsigned short a) const
{
	return htons(a);
}

unsigned short Autumn::networking::Port::to_host(unsigned short a) const
{
	return ntohs(a);
}