#pragma once

#if defined(_WIN32)
#include <winsock2.h>
#include <ws2ipdef.h>
#pragma comment( lib, "wsock32.lib" )
#include <WS2tcpip.h>
#pragma comment(lib,  "ws2_32.lib") 

#elif defined(__APPLE__) || defined(__unix__ )

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <errno.h>

#endif


#if defined(_WIN32)
#define PRINT_LAST_ERROR(message) printf(message##"\n\tERROR: %i", WSAGetLastError())
#define LAST_ERROR_CODE WSAGetLastError()
#elif  defined(__APPLE__) || defined(__unix__ )
#define PRINT_LAST_ERROR(message)  printf(message##"\n\tERROR: %i", errno)
#define LAST_ERROR_CODE errno
#endif