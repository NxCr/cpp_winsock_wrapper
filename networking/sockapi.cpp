#pragma once
#include "lib_includes.cpp"
#include "wsa_singleton.cpp"
#include <exception>
#include <iostream>
#include "ip_endpoint.h"
#define THROW_SOCKET_EXCEPTION(str) throw std::runtime_error(std::string("There was an error while trying to ") + std::string(##str) + std::string(" error code: " ) + std::to_string(LAST_ERROR_CODE))
#define CHECK_OPEN_STATE(msg) if (this->is_open_){} else throw std::runtime_error("Error. The socket has to be opened before calls to" ##msg)
#define CHECK_CONNECTED_STATE(msg) if (this->is_connected_){} else throw std::runtime_error("Error. The socket has to be connected before calls to" ##msg)

namespace Autumn::networking
{
		inline void initialize_sockets()
		{
			networking::WSA::Startup();
		}
		inline void shutdown_sockets()
		{
			networking::WSA::Cleanup();
		}


		inline void close_socket(int socket)
		{
			closesocket(socket);
		}

		template <IP protocol>
		inline bool create_udp_socket(int* handle)
		{
			if ((*handle = (int)socket((unsigned short)protocol, SOCK_DGRAM, IPPROTO_UDP)) != INVALID_SOCKET)
				return true;
			return false;
		}

		template <IP protocol>
		inline bool create_tcp_socket(int* handle)
		{

			if ((*handle = (int)socket((unsigned short)protocol, SOCK_STREAM, IPPROTO_TCP)) != INVALID_SOCKET)
				return true;
			return false;
		}

		template <networking::IP protocol>
		inline bool bind_socket(int handle, IPEndPoint<protocol> const& endpoint)
		{
			if (bind(handle, reinterpret_cast<const sockaddr*>(&endpoint), sizeof(IPEndPoint<protocol>)) == 0)
				return true;
			return false;
		}

		template<networking::IP protocol>
		inline bool connect(int handle, IPEndPoint<protocol> const& endpoint)
		{
			if (connect(handle, reinterpret_cast<const sockaddr*>(&endpoint), sizeof(IPEndPoint<protocol>)) == 0)
				return true;
			return false;
		}

		//template <SocketMode mode>
		//inline bool set_non_blocking(int handle) {throw std::runtime_error("Not implemented. Wrong template chosen by compiler");};

		//template<>
		//inline bool set_non_blocking<SocketMode::BLOCKING>(int handle)
		//{
		//	return true;
		//}

		//template<>
		//inline bool set_non_blocking<SocketMode::NON_BLOCKING>(int handle)
		//{
		//	DWORD nonBlocking = 1;
		//	return ioctlsocket(handle, FIONBIO, &nonBlocking) == 0;
		//}

		template<IP protocol>
		inline int send_to(int handle, const char* packet_data, unsigned int packet_size, IPEndPoint<protocol> const& address)
		{
			return sendto(handle, packet_data, packet_size, 0, reinterpret_cast<const sockaddr*>(&address), sizeof(IPEndPoint<protocol>) );
		}

		template<IP protocol>
		inline int receive_from(int socket, char* packet_data, int max_packet_size, IPEndPoint<protocol>& address)
		{
			int size = sizeof(IPEndPoint<protocol>);
			return recvfrom(socket, packet_data, max_packet_size, 0, reinterpret_cast<sockaddr*>(&address), &size);
		}

		inline int send_tcp(int handle, const char* packet_data, unsigned int packet_size)
		{
			return send(handle, packet_data, packet_size, 0);
		}

		inline int receive(int socket, char* packet_data, int max_packet_size)
		{
			return recv(socket, packet_data, max_packet_size, 0);
		}

		inline int check_data_available(int socket)
		{
			unsigned long count;
			ioctlsocket(socket, FIONREAD, &count);
			return (int)count;
		}
}