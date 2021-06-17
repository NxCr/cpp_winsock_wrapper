#pragma once
#include "lib_includes.cpp"
#include <stdexcept>

namespace Bunny
{
	namespace networking
	{
		static int reference_count_ = 0;
		class WSA
		{
		public:
			static void Startup()
			{
			#if defined(_WIN32)
				if (reference_count_ == 0)
				{
					WSADATA WsaData; \
						if (WSAStartup(MAKEWORD(2, 2), &WsaData) != NO_ERROR)\
							throw std::runtime_error("could not start up WSA");
				}
				++reference_count_;
			#endif
			}
			static void Cleanup()
			{
			#if defined(_WIN32)
				--reference_count_;
				if (reference_count_ == 0)
					WSACleanup();
				//else if (reference_count_ < 0)
				//	throw std::runtime_error("WSA reference count sub 0");
			#endif
			}
		private:
			WSA() = delete;
			~WSA() {}
		};
		class WSAGuard
		{
		public:
			WSAGuard()
			{
				WSA::Startup();
			}
			~WSAGuard()
			{
				WSA::Cleanup();
			}
		};
	}
}
