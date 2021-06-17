#pragma once
namespace Autumn {
	namespace networking {
		enum class IP : unsigned short {
			v4 = 2, // AF_INET
			v6 = 23 // AF_INET6
		};
	}
	using internet_protocol = networking::IP;
}