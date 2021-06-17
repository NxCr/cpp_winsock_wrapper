#include "tcp_client.h"
#include "sockapi.cpp"
namespace Autumn::networking
{
	template<> TcpClient<IP::v4>::TcpClient() : is_open_{ false }, is_connected_{ false }, handle_{ (int)INVALID_SOCKET }
	{
		initialize_sockets(); 
	}

	template<> TcpClient<IP::v6>::TcpClient() : is_open_{ false }, is_connected_{ false }, handle_{ (int)INVALID_SOCKET }
	{
		initialize_sockets(); 
	}

	template<> TcpClient<IP::v4>& TcpClient<IP::v4>::operator=(TcpClient<IP::v4>&& other) noexcept
	{
		this->handle_ = other.handle_;
		this->is_open_ = other.is_open_;
		this->is_connected_ = other.is_connected_;

		other.handle_ = (int)INVALID_SOCKET;
		other.is_connected_ = false;
		other.is_open_ = false;

		return *this;
	}
	template<> TcpClient<IP::v6>& TcpClient<IP::v6>::operator=(TcpClient<IP::v6>&& other) noexcept
	{
		this->handle_ = other.handle_;
		this->is_open_ = other.is_open_;
		this->is_connected_ = other.is_connected_;

		other.handle_ = (int)INVALID_SOCKET;
		other.is_connected_ = false;
		other.is_open_ = false;

		return *this;
	}

	template<> TcpClient<IP::v4>::~TcpClient()
	{
		if (is_open_)
			Close();
		shutdown_sockets();
	}
	template<> TcpClient<IP::v6>::~TcpClient() 
	{
		if (is_open_)
			Close();
		shutdown_sockets();
	}

	template<> void TcpClient<IP::v4>::Open()
	{
		if (this->is_open_) throw std::runtime_error("Socket was already open");

		if (!create_tcp_socket<IP::v4>(&this->handle_))
			THROW_SOCKET_EXCEPTION("open a tcp socket.");
		this->is_open_ = true;
	}
	template<> void TcpClient<IP::v6>::Open()
	{
		if (this->is_open_) throw std::runtime_error("Socket was already open");

		if (!create_tcp_socket<IP::v6>(&this->handle_))
			THROW_SOCKET_EXCEPTION("open a tcp socket.");
		this->is_open_ = true;
	}

	template<> void TcpClient<IP::v4>::Bind(IPEndPoint<IP::v4> const& endpoint)
	{
		CHECK_OPEN_STATE("Bind");
		if (!bind_socket(handle_, endpoint))
			THROW_SOCKET_EXCEPTION("bind the socket.");
		is_open_ = true;
	}
	template<> void TcpClient<IP::v6>::Bind(IPEndPoint<IP::v6> const& endpoint)
	{
		CHECK_OPEN_STATE("Bind");
		if (!bind_socket(handle_, endpoint))
			THROW_SOCKET_EXCEPTION("bind the socket.");
		is_open_ = true;
	}
	
	template<> void TcpClient<IP::v4>::Close()
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
	template<> void TcpClient<IP::v6>::Close()
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
	
	template<> void TcpClient<IP::v4>::Connect(IPEndPoint<IP::v4> const& endpoint)
	{
		CHECK_OPEN_STATE("Connect");
		if (is_connected_) throw std::runtime_error("Client already connected.");
		if (!connect(handle_, endpoint)) THROW_SOCKET_EXCEPTION("connecting.");
		is_connected_ = true;

	}
	template<> void TcpClient<IP::v6>::Connect(IPEndPoint<IP::v6> const& endpoint)
	{
		CHECK_OPEN_STATE("Connect");
		if (is_connected_) throw std::runtime_error("Client already connected.");
		if (!connect(handle_, endpoint)) THROW_SOCKET_EXCEPTION("connecting.");
		is_connected_ = true;

	}
	
	template<> int TcpClient<IP::v4>::Send(const char* data, int size)
	{
		CHECK_OPEN_STATE("Send");
		CHECK_CONNECTED_STATE("Send");
		int ret = send_tcp(this->handle_, data, size);
		if (ret == SOCKET_ERROR)
			THROW_SOCKET_EXCEPTION("send data.");
		return ret;
	}
	template<> int TcpClient<IP::v6>::Send(const char* data, int size)
	{
		CHECK_OPEN_STATE("Send");
		CHECK_CONNECTED_STATE("Send");
		int ret = send_tcp(this->handle_, data, size);
		if (ret == SOCKET_ERROR)
			THROW_SOCKET_EXCEPTION("send data.");
		return ret;
	}
	
	template<> int TcpClient<IP::v4>::Receive(char* data, int size)
	{
		CHECK_OPEN_STATE("Receive");
		CHECK_CONNECTED_STATE("Receive");
		int ret = receive(handle_, data, size);
		if (ret  == SOCKET_ERROR)
			THROW_SOCKET_EXCEPTION("Receive");
		return ret;
	}
	template<> int TcpClient<IP::v6>::Receive(char* data, int size)
	{
		CHECK_OPEN_STATE("Receive");
		CHECK_CONNECTED_STATE("Receive");
		int ret = receive(handle_, data, size);
		if (ret == SOCKET_ERROR)
			THROW_SOCKET_EXCEPTION("Receive");
		return ret;
	}
	
	template<> int TcpClient<IP::v4>::BytesAvailable() const
	{
		CHECK_OPEN_STATE("BytesAvailable");
		CHECK_CONNECTED_STATE("BytesAvailable");
		return check_data_available(handle_);
	}
	template<> int TcpClient<IP::v6>::BytesAvailable() const
	{
		CHECK_OPEN_STATE("BytesAvailable");
		CHECK_CONNECTED_STATE("BytesAvailable");
		return check_data_available(handle_);
	}
	
	template<> int TcpClient<IP::v4>::GetLastError()
	{
		return LAST_ERROR_CODE;
	}
	template<> int TcpClient<IP::v6>::GetLastError()
	{
		return LAST_ERROR_CODE;
	}
}