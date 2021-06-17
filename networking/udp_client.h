#pragma once
#include "ip_protocols.h"
#include "ip_endpoint.h"

namespace Autumn
{
	namespace networking
	{
		template <IP protocol>
		class UdpClient
		{
		public:
			// Sets up the memory
			UdpClient();

			// moves the client. Other client is left ded
			UdpClient(UdpClient<protocol>&& other) 
			{
				*this = std::move(other); 
			}
			UdpClient<protocol>& operator=(UdpClient<protocol>&& other) noexcept;

			// deleted.
			UdpClient(UdpClient<protocol> const& other) = delete;
			UdpClient<protocol>& operator=(UdpClient<protocol> const& other) = delete;

			// cleans up the socket layer etc.
			~UdpClient();

			// creates the underlying socket and sets up the socket layer
			void Open();

			// closes the socket
			void Close();

			// Checks if the socket is open
			bool IsOpen() const { return is_open_; }

			// sends the data to the given endpoint
			int Send(IPEndPoint<protocol> const& destination,
					 const char* data,
					 int size);

			// receives data and puts it into the buffer, returning the number of bytes received
			int Receive(IPEndPoint<protocol>& sender,
						char* data,
						int size);

			// tells how many bytes are available
			int BytesAvailable() const;

			// binds the socket to the given local endpoint
			void Bind(IPEndPoint<protocol> const& endpoint = { IPAddress<protocol>::Any(), Port::Unspecified() });

			// returns last error code received (for debug purposes)
			int GetLastError();

		private:
			int handle_;
			bool is_open_;
		};
	}
	using udp_client = networking::UdpClient<networking::IP::v4>;
}