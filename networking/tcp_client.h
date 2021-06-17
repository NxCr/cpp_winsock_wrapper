#pragma once
#include "ip_protocols.h"
#include "ip_endpoint.h"
#include <iostream>

namespace Bunny
{
	namespace networking
	{
		template <IP protocol>
		class TcpClient
		{
		public:
			// Sets up the memory
			TcpClient();

			// moves the client. Other client is left ded
			TcpClient(TcpClient<protocol>&& other)
			{
				*this = std::move(other);
			}
			TcpClient<protocol>& operator=(TcpClient<protocol>&& other) noexcept;

			// deleted.
			TcpClient(TcpClient<protocol> const& other) = delete;
			TcpClient<protocol>& operator=(TcpClient<protocol> const& other) = delete;


			// cleans up the socket layer etc.
			~TcpClient();

			// creates the underlying socket and sets up the socket layer
			void Open();

			// binds the socket to the given local endpoint
			void Bind(IPEndPoint<protocol> const& endpoint = { IPAddress<protocol>::Any(), Port::Unspecified() });

			// closes the socket
			void Close();

			// connects the socket to a remote endpoint
			void Connect(IPEndPoint<protocol> const& endpoint);

			// sends the data to the connected endpoint
			int Send(const char* data,
					  int size);

			// receives data and puts it into the buffer, returning the number of bytes received
			int Receive(char* data,
						int size);

			// tells how many bytes are available
			int BytesAvailable() const;
		
			// Checks if the socket is open
			bool IsOpen() const { return is_open_; }

			// Checks if the socket is open
			bool IsConnected() const { return is_connected_; }

			// returns last error code received (for debug purposes)
			int GetLastError();

		private:
			template<IP rotocol>
			friend class TcpListener;
			int handle_;
			bool is_open_;
			bool is_connected_;
		};
	}
	using tcp_client = networking::TcpClient<networking::IP::v4>;
}