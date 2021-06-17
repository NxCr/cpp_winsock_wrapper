#pragma once

#include <string>
#include <sstream>
#include <iomanip>

#include "../ip_protocols.h"

namespace Bunny::networking
{
	// Class template with exactly 2 specializations. Represents an address in the correct format
	template <IP protocolType>
	class IPAddress {};

	template<>
	class IPAddress<IP::v4>
	{
	public:
		constexpr IPAddress() noexcept : value_{ Any().value_ } {}
		IPAddress(std::string const& hostname);
		IPAddress(const char* hostname) : IPAddress(std::string(hostname)) {};

		static constexpr IPAddress<IP::v4> Any() noexcept { return { (unsigned long)0x00000000U }; }
		static constexpr IPAddress<IP::v4> Loopback() noexcept { return { (unsigned long)0x7f000001U }; }
		static constexpr IPAddress<IP::v4> Localhost() noexcept { return { (unsigned long)0x7f000001U }; }

		std::string to_string() const
		{
			std::stringstream sstr;
			sstr << ip0_ + 0 << "." << ip1_ + 0 << "." << ip2_ + 0 << "." << ip3_ + 0;
			return sstr.str();
		}

	private:
		constexpr IPAddress(unsigned long value) noexcept : value_{ value } {}

	private:
		union
		{
			unsigned long value_;
			struct
			{
				unsigned char ip0_, ip1_, ip2_, ip3_;
			};
		};

	};

	template<>
	class IPAddress<IP::v6>
	{
	public:
		constexpr IPAddress() : value_{ 0 } {}
		IPAddress(std::string const& hostname);
		IPAddress(const char* hostname) : IPAddress(std::string(hostname)) {};

		static constexpr IPAddress<IP::v6> Any() noexcept { return { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 }; }
		static constexpr IPAddress<IP::v6> Loopback() noexcept { return { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 }; }
		static constexpr IPAddress<IP::v6> Localhost() noexcept { return { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 }; }

		std::string to_string() const
		{
			std::stringstream ss;
			ss << std::hex << std::setfill('0');
			for (int i = 0; i < 16; i += 2)
			{
				ss << std::setw(2) << static_cast<unsigned>(value_[i]) << std::setw(2) << static_cast<unsigned>(value_[i + 1]) << ":";
			}
			return ss.str();
		}

	private:
		constexpr IPAddress(unsigned char v1, unsigned char v2,
							unsigned char v3, unsigned char v4,
							unsigned char  v5, unsigned char  v6,
							unsigned char  v7, unsigned char  v8,
							unsigned char v9, unsigned char v10,
							unsigned char v11, unsigned char v12,
							unsigned char  v13, unsigned char  v14,
							unsigned char  v15, unsigned char  v16)
			: value_{ v1,v2,v3,v4,v5,v6,v7, v8, v9, v10, v11, v12, v13, v14, v15, v16 }
		{}
	private:
		unsigned char value_[16];
	};

}