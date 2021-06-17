#pragma once
#include "udp_client.h"
#include "sockapi.cpp"

namespace Bunny
{
	namespace networking
	{
		template<> UdpClient<IP::v4>::UdpClient() : is_open_{ false }, handle_{ (int)INVALID_SOCKET }
		{
			initialize_sockets();
		}
		template<> UdpClient<IP::v6>::UdpClient() : is_open_{ false }, handle_{ (int)INVALID_SOCKET }
		{
			initialize_sockets();
		}

		template<> UdpClient<IP::v4>& UdpClient<IP::v4>::operator=(UdpClient<IP::v4>&& other) noexcept
		{
			this->handle_ = other.handle_;
			this->is_open_ = other.is_open_;

			other.handle_ = (int)INVALID_SOCKET;
			other.is_open_ = false;

			return *this;
		}
		template<> UdpClient<IP::v6>& UdpClient<IP::v6>::operator=(UdpClient<IP::v6>&& other) noexcept
		{
			this->handle_ = other.handle_;
			this->is_open_ = other.is_open_;

			other.handle_ = (int)INVALID_SOCKET;
			other.is_open_ = false;

			return *this;
		}

		template<> UdpClient<IP::v4>::~UdpClient()
		{
			if (is_open_)
				Close();
			shutdown_sockets();
		}
		template<> UdpClient<IP::v6>::~UdpClient()
		{
			if (is_open_)
				Close();
			shutdown_sockets();
		}

		template<> int UdpClient<IP::v4>::BytesAvailable() const
		{
			CHECK_OPEN_STATE("BytesAvailable");
			return check_data_available(handle_);
		}	
		template<> int UdpClient<IP::v6>::BytesAvailable() const
		{
			CHECK_OPEN_STATE("BytesAvailable");
			return check_data_available(handle_);
		}

		template<> void UdpClient<IP::v4>::Bind(IPEndPoint<IP::v4> const& endpoint)
		{
			CHECK_OPEN_STATE("Bind");
			if (!bind_socket(handle_, endpoint))
				THROW_SOCKET_EXCEPTION("bind the socket.");
			is_open_ = true;
		}		
		template<> void UdpClient<IP::v6>::Bind(IPEndPoint<IP::v6> const& endpoint)
		{
			CHECK_OPEN_STATE("Bind");
			if (!bind_socket(handle_, endpoint))
				THROW_SOCKET_EXCEPTION("bind the socket.");
			is_open_ = true;
		}

		template<> void UdpClient<IP::v4>::Close()
		{
			if (is_open_)
			{
				is_open_ = false;
				close_socket(handle_);
			}
			else
			{
				throw std::runtime_error("Socket has already been closed or wasn't open");
			}
		}		
		template<> void UdpClient<IP::v6>::Close()
		{
			if (is_open_)
			{
				is_open_ = false;
				close_socket(handle_);
			}
			else
			{
				throw std::runtime_error("Socket has already been closed or wasn't open");
			}
		}
		
		template<> int UdpClient<IP::v4>::GetLastError()
		{
			return LAST_ERROR_CODE;
		}		
		template<> int UdpClient<IP::v6>::GetLastError()
		{
			return LAST_ERROR_CODE;
		}

		template<> void UdpClient<IP::v4>::Open()
		{
			if (this->is_open_) throw std::runtime_error("Socket was already open");

			if (!create_udp_socket<IP::v4>(&this->handle_))
				THROW_SOCKET_EXCEPTION("open a udp socket.");
			this->is_open_ = true;
		}		
		template<> void UdpClient<IP::v6>::Open()
		{
			if (this->is_open_) throw std::runtime_error("Socket was already open");

			if (!create_udp_socket<IP::v6>(&this->handle_))
				THROW_SOCKET_EXCEPTION("open a udp socket.");
			this->is_open_ = true;
		}

		template<> int UdpClient<IP::v4>::Send(IPEndPoint<IP::v4> const& destination, const char* data, int size)
		{
			CHECK_OPEN_STATE("Send");
			int ret = send_to(this->handle_, data, size, destination);
			if (ret == SOCKET_ERROR)
				THROW_SOCKET_EXCEPTION("send data.");
			return ret;
		}	
		template<> int UdpClient<IP::v6>::Send(IPEndPoint<IP::v6> const& destination, const char* data, int size)
		{
			CHECK_OPEN_STATE("Send");
			int ret = send_to(this->handle_, data, size, destination);
			if (ret == SOCKET_ERROR)
				THROW_SOCKET_EXCEPTION("send data.");
			return ret;
		}
		
		template<> int UdpClient<IP::v4>::Receive(IPEndPoint<IP::v4>& sender, char* data, int size)
		{
			CHECK_OPEN_STATE("Receive");
			int res = receive_from(this->handle_, data, size, sender);
			if (res == SOCKET_ERROR) THROW_SOCKET_EXCEPTION("receive data.");
			return res;
		}
		template<> int UdpClient<IP::v6>::Receive(IPEndPoint<IP::v6>& sender, char* data, int size)
		{
			CHECK_OPEN_STATE("Receive");
			int res = receive_from(this->handle_, data, size, sender);
			if (res == SOCKET_ERROR) THROW_SOCKET_EXCEPTION("receive data.");
			return res;
		}
	}
}



//namespace Bunny {
//	namespace networking {
//
//		bool UdpClient::Open(unsigned short port, IP protocol)
//		{
//			if (is_open_)
//				return false;
//			if (!create_udp_socket(&handle_, protocol))
//				return false;
//			if (!bind_socket(handle_, port, protocol)) {
//				close_socket(handle_);
//				return false;
//			}
//			if (mode_ == SocketMode::NON_BLOCKING)
//			{
//				if (!set_non_blocking(handle_))
//				{
//					close_socket(handle_);
//					return false;
//				}
//			}
//			protocol_ = protocol;
//			is_open_ = true;
//			return true;
//		}
//
//		void UdpClient::Open(IPEndPoint const& ep)
//		{
//			if (is_open_)
//				throw SocketException("The socket is already open.");
//			if (!create_udp_socket(&handle_, ep.Protocol()))
//				THROW_SOCKET_EXCEPTION("open the socket.");
//			if (!bind_socket(handle_, reinterpret_cast<sockaddr*>(ep.saddr_), ep.Protocol() == IP::v4 ? sizeof(sockaddr_in) : sizeof(sockaddr_in6))) {
//				close_socket(handle_);
//				THROW_SOCKET_EXCEPTION("bind the socket.");
//			}
//			if (mode_ == SocketMode::NON_BLOCKING)
//			{
//				if (!set_non_blocking(handle_))
//				{
//					close_socket(handle_);
//					THROW_SOCKET_EXCEPTION("set the socket to non-blocking io mode.");
//				}
//			}
//			protocol_ = ep.protocol_;
//			is_open_ = true;
//		}
//
//		void UdpClient::Close()
//		{
//			if (is_open_) {
//				close_socket(handle_);
//				is_open_ = false;
//			}
//		}
//
//		bool UdpClient::DataAvailable() const
//		{
//			return check_data_available(handle_) > 0;
//		}
//
//		bool UdpClient::IsOpen() const
//		{
//			return is_open_;
//		}
//
//		int UdpClient::BytesAvailable() const
//		{
//			return check_data_available(handle_);
//		}
//
//		bool UdpClient::Send(IPEndPoint const& destination,
//			const char * data,
//			int size)
//		{
//			if (!is_open_)
//				throw SocketException("The socket is not open.");
//			if (protocol_ != destination.protocol_)
//				throw SocketException("The specified endpoint is not using the same internet protocol");
//			return send_to(handle_, data, size, reinterpret_cast<sockaddr*>(destination.saddr_), size_of_protocol(destination.protocol_)) == size;
//		}
//
//		int UdpClient::Receive(IPEndPoint & sender,
//			char * data,
//			int size)
//		{
//			int si=sizeof(sockaddr);
//			int bytes_received = receive_from(handle_, data, size, reinterpret_cast<sockaddr*>(sender.saddr_), &si);
//			switch (reinterpret_cast<sockaddr*>(sender.saddr_)->sa_family) {
//			case AF_INET:
//				sender.protocol_ = IP::v4;
//				break;
//			case AF_INET6:
//				sender.protocol_ = IP::v6;
//				break;
//			}
//
//			return bytes_received >= 0 ? bytes_received : 0;
//		}
//	}
//
//}
//#undef THROW_SOCKET_EXCEPTION