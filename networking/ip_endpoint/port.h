#pragma once

namespace Autumn::networking
{
	// Represents a port number. Takes care of endianness
	class Port
	{
	public:
		inline Port(unsigned short value) noexcept :value_{ to_network(value) } {};
		inline constexpr Port() noexcept :value_{ 0 } {};

		static constexpr Port Unspecified() { return { }; }

		unsigned short host_order() const { return to_host(value_); }
		unsigned short network_order() const { return value_; }

	private:

		unsigned short to_network(unsigned short a) const;
		unsigned short to_host(unsigned short a) const;

	private:
		unsigned short value_;
	};
}