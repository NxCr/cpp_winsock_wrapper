#pragma once
#include "ip_address.h"
#include "../lib_includes.cpp"
#include "../wsa_singleton.cpp"

Bunny::networking::IPAddress<Bunny::networking::IP::v6>::IPAddress(std::string const& hostname)
	:value_{ 0 }
{
	WSAGuard wsa_;

	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	struct addrinfo* res;
	hints.ai_family = AF_INET6;
	volatile int errc;
	struct addrinfo* ptr;
	errc = getaddrinfo(hostname.c_str(), NULL, &hints, &res);
	if (errc) throw std::runtime_error("Error, couldn't get address info for " + std::string(hostname) + ", errc: " + std::to_string(errc));
	ptr = res;
	{
		while (ptr)
		{
			if (ptr->ai_family == AF_INET6)
			{
				auto saddr = (*(struct sockaddr_in6*)ptr->ai_addr).sin6_addr.u.Byte;
				for (int i = 0; i < 16; i++)
					value_[i] = saddr[i];
				freeaddrinfo(res);
				return;
			}
			ptr = ptr->ai_next;
		}
	}
	freeaddrinfo(res);
	throw std::runtime_error("Error, no suitable address found for " + std::string(hostname));
}
Bunny::networking::IPAddress<Bunny::networking::IP::v4>::IPAddress(std::string const& hostname)
	:value_{ 0 }
{
	WSAGuard wsa_;

	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	struct addrinfo* res;
	hints.ai_family = AF_INET;
	volatile int errc;
	struct addrinfo* ptr;
	errc = getaddrinfo(hostname.c_str(), NULL, &hints, &res);
	if (errc) throw std::runtime_error("Error, couldn't get address info for " + std::string(hostname) + ", errc: " + std::to_string(errc));
	ptr = res;
	while (ptr)
	{
		if (ptr->ai_family == AF_INET)
		{
			value_ = (*(struct sockaddr_in*)ptr->ai_addr).sin_addr.S_un.S_addr;
			freeaddrinfo(res);
			return;
		}
		ptr = ptr->ai_next;
	}
	freeaddrinfo(res);
	throw std::runtime_error("Error, no suitable address found for " + std::string(hostname));
}
