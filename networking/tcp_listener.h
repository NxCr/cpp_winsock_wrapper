#pragma once
#include "ip_protocols.h"
#include "ip_endpoint.h"
#include "tcp_client.h"

namespace Bunny
{
	namespace networking
	{
		template <IP protocol>
		class TcpListener
		{
		public:
			// sets up the memory
			TcpListener();

			// moves the client. Other client is left ded
			TcpListener(TcpListener<protocol>&& other)
			{
				*this = std::move(other);
			}
			TcpListener<protocol>& operator=(TcpListener<protocol>&& other) noexcept;

			// deleted.
			TcpListener(TcpClient<protocol> const& other) = delete;
			TcpListener<protocol>& operator=(TcpClient<protocol> const& other) = delete;
			
			// cleans up the socket layer etc.
			~TcpListener();

			// creates the underlying socket and sets up the socket layer
			void Open();

			// binds the socket to the given local endpoint
			void Bind(IPEndPoint<protocol> const& endpoint);

			// closes the socket
			void Close();

			// connects the socket to a remote endpoint
			void StartListening();

			// blocking call, tries to accept a thing
			void Accept(TcpClient<protocol>& out);

			// Checks if the socket is open
			bool IsOpen() const { return is_open_; }

			// Checks if the socket is open
			bool IsListening() const { return is_listening_; }

			// returns last error code received (for debug purposes)
			int GetLastError();
		private:
			int handle_;
			int is_open_;
			int is_listening_;
		};
	}
	using tcp_listener = networking::TcpListener<networking::IP::v4>;
}