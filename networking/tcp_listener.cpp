#include "tcp_listener.h"
#include "sockapi.cpp"

namespace Autumn::networking
{
	template<> TcpListener<IP::v4>::TcpListener() :handle_{ (int)INVALID_SOCKET }, is_open_{ false }, is_listening_{ false }
	{
		initialize_sockets(); 
	}
	template<> TcpListener<IP::v6>::TcpListener() :handle_{ (int)INVALID_SOCKET }, is_open_{ false }, is_listening_{ false }
	{
		initialize_sockets(); 
	}

	template<> TcpListener<IP::v4>& TcpListener<IP::v4>::operator=(TcpListener<IP::v4>&& other) noexcept
	{
		this->handle_ = other.handle_;
		this->is_open_ = other.is_open_;
		this->is_listening_ = other.is_listening_;

		other.handle_ = (int)INVALID_SOCKET;
		other.is_listening_ = false;
		other.is_open_ = false;

		return *this;
	}
	template<> TcpListener<IP::v6>& TcpListener<IP::v6>::operator=(TcpListener<IP::v6>&& other) noexcept
	{
		this->handle_ = other.handle_;
		this->is_open_ = other.is_open_;
		this->is_listening_ = other.is_listening_;

		other.handle_ = (int)INVALID_SOCKET;
		other.is_listening_ = false;
		other.is_open_ = false;

		return *this;
	}

	template<> TcpListener<IP::v4>::~TcpListener()
	{
		if (is_open_)
			Close();
		shutdown_sockets();
	}
	template<> TcpListener<IP::v6>::~TcpListener()
	{
		if (is_open_)
			Close();
		shutdown_sockets();
	}

	template<> void TcpListener<IP::v4>::Open()
	{
		if (this->is_open_) throw std::runtime_error("Socket was already open");

		if (!create_tcp_socket<IP::v4>(&this->handle_))
			THROW_SOCKET_EXCEPTION("open a tcp socket.");
		this->is_open_ = true;
	}
	template<> void TcpListener<IP::v6>::Open()
	{
		if (this->is_open_) throw std::runtime_error("Socket was already open");

		if (!create_tcp_socket<IP::v4>(&this->handle_))
			THROW_SOCKET_EXCEPTION("open a tcp socket.");
		this->is_open_ = true;
	}
	
	template<> void TcpListener<IP::v4>::Bind(IPEndPoint<IP::v4> const& endpoint)
	{
		CHECK_OPEN_STATE("Bind");
		if (!bind_socket(handle_, endpoint))
			THROW_SOCKET_EXCEPTION("bind the socket.");
		is_open_ = true;
	}
	template<> void TcpListener<IP::v6>::Bind(IPEndPoint<IP::v6> const& endpoint)
	{
		CHECK_OPEN_STATE("Bind");
		if (!bind_socket(handle_, endpoint))
			THROW_SOCKET_EXCEPTION("bind the socket.");
		is_open_ = true;
	}

	template<> void TcpListener<IP::v4>::Close()
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
	template<> void TcpListener<IP::v6>::Close()
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

	template<> void TcpListener<IP::v4>::StartListening()
	{
		CHECK_OPEN_STATE("Listen");
		if (is_listening_) throw std::runtime_error("Listener is already listening.");
		if (listen(handle_, SOMAXCONN) == SOCKET_ERROR) THROW_SOCKET_EXCEPTION("Listen");
		is_listening_ = true;
	}
	template<> void TcpListener<IP::v6>::StartListening()
	{
		CHECK_OPEN_STATE("Listen");
		if (is_listening_) throw std::runtime_error("Listener is already listening.");
		if (listen(handle_, SOMAXCONN) == SOCKET_ERROR) THROW_SOCKET_EXCEPTION("Listen");
		is_listening_ = true;
	}

	template<> void TcpListener<IP::v4>::Accept(TcpClient<IP::v4>& out)
	{
		int accepted_handle_ = accept(handle_, NULL, NULL);
		if (accepted_handle_ == INVALID_SOCKET) THROW_SOCKET_EXCEPTION("Accept");
		out.handle_ = accepted_handle_;
		out.is_open_ = true;
		out.is_connected_ = true;
	}
	template<> void TcpListener<IP::v6>::Accept(TcpClient<IP::v6>& out)
	{
		int accepted_handle_ = accept(handle_, NULL, NULL);
		if (accepted_handle_ == INVALID_SOCKET) THROW_SOCKET_EXCEPTION("Accept");
		out.handle_ = accepted_handle_;
		out.is_open_ = true;
		out.is_connected_ = true;
	}

	template<> int TcpListener<IP::v4>::GetLastError()
	{
		return LAST_ERROR_CODE;
	}
	template<> int TcpListener<IP::v6>::GetLastError()
	{
		return LAST_ERROR_CODE;
	}
}