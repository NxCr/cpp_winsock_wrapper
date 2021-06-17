#pragma once
#include <string>
#include <sstream>
#include <iomanip>

#include "ip_protocols.h"
#include "ip_endpoint/ip_address.h"
#include "ip_endpoint/port.h"

namespace Autumn
{
	namespace networking
	{
		// class template with exactly 2 specializations
		// Either ipv4 or ipv6 = matches the memory layout of saddr_in and saddr_in6 respectively
		template <IP protocolType> class IPEndPoint {};

		// IPv4 Endpoint. Has the exact memory layout of saddr_in
		template <> class IPEndPoint<IP::v4>
		{
		public:
			constexpr IPEndPoint(
				IPAddress<IP::v4> address = IPAddress<IP::v4>::Any(),
				Port port = Port::Unspecified()
				);

			std::string to_string() const
			{
				return "IP::v4 " + address_.to_string() + ":" + std::to_string(port_.host_order());
			}

		private:
			IP family_;													
			Port port_;													
			IPAddress<IP::v4> address_;									
			char padding_[8];
		};

		// IPv6 Endpoint. Has the exact memory layour of saddr_in6
		template<> class IPEndPoint<IP::v6>
		{
		public:
			constexpr IPEndPoint(
				IPAddress<IP::v6> address = IPAddress<IP::v6>::Any(),
				Port port = Port::Unspecified()
				);

			std::string to_string() const
			{
				return "IP::v6 " + address_.to_string() + ":" + std::to_string(port_.host_order());
			}

		private:
			IP family_;
			Port port_;
			unsigned long  flowinfo_padding_;       
			IPAddress<IP::v6> address_; 
			unsigned long scope_id_padding_; 
		};

	#pragma region Definitions
		constexpr IPEndPoint<IP::v4>::IPEndPoint(IPAddress<IP::v4> address, Port port) :
			family_{ IP::v4 }, port_{ port }, address_{ address }, padding_{ 0,0,0,0,0,0,0,0 } {};



		constexpr IPEndPoint<IP::v6>::IPEndPoint(IPAddress<IP::v6> address, Port port) :
			family_{ IP::v6 }, port_{ port }, address_{ address }, flowinfo_padding_{ 0 }, scope_id_padding_{ 0 } {};
		
	#pragma endregion Definitons
}
	
	using ip_endpoint = networking::IPEndPoint<networking::IP::v4>;
	using ip_address = networking::IPAddress<networking::IP::v4>;
	using port_type = networking::Port;
}