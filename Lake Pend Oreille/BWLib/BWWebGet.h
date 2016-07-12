//  BWWebGet.h
//  Get Objects from Web via HTTP
//  by Bill Weinman <http://bw.org/>
//  Copyright (c) 2014 The BearHeart Group LLC
//
//	2014-05-09 -- support for Windows sockets (Winsock)
//

#ifndef __BWLib__BWWebGet__
#define __BWLib__BWWebGet__

#include "BWString.h"
#include "BWNumber.h"

#include <cstdio>
#include <vector>
#include <fcntl.h>
#include <errno.h>

#define __BWWebGet_VERSION "1.1.3"
#define __BWWebGet_max_buf 16384

#ifdef _MSC_VER     // for Winsock

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#else	// NOT _MSC_ver

#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>		// for legacy sockets
#include <netdb.h>
#include <arpa/inet.h>

#endif	// _MSC_VER

static const char * _bwwg_ua_string = "BWWebGet";
static const int _bwwg_timeout_secs = 10;
static const size_t _bwwg_max_buf = __BWWebGet_max_buf;
static const int _bwwg_web_port = 80;
static const int _bwwg_failure = -1;

class BWWebGet {
	const BWString _crlf = "\r\n";
	const BWString _newline = "\n";
	
	BWString _host;
	BWNumber _port;
	BWNumber _http_status_code;

	BWString _header;
	BWString _body;
	
	char _buffer[__BWWebGet_max_buf];
	char _inet_addr[INET6_ADDRSTRLEN];

	int _errno = 0;
	const char * _errstr = nullptr;

#ifdef _MSC_VER
#define _STRERROR_SIZE 63
	char __errstr[_STRERROR_SIZE + 1];	// windows requires a buffer
    WORD _wsaVersionRequested = MAKEWORD(1, 1);
    WSADATA _wsa_data;
#endif

	int _socket = 0;	// socket file descriptor
	bool _have_socket = false;
	
	// rule of three
	BWWebGet( BWWebGet & );						// no copy constructor
	BWWebGet operator = ( BWWebGet & );			// no assignment operator
	BWWebGet();									// no default constructor

	void socketclose();
	void _set_error();
	void _reset_error();
	void _set_http_status_code();
	void * _get_in_addr(struct sockaddr * sa);
	void _set_blocking( bool f );
	int _connect();
	void _send( const char * msg );
	void _receive();

public:
	static const char * version() { return __BWWebGet_VERSION; }
	BWWebGet( const BWString & host );
	BWWebGet( const BWString & host, const BWNumber & port );
	~BWWebGet();

	void reset();
	const int get_errno() const { return _errno; }
	const char * get_errstr() const { return _errstr; }
	const char * get_address() const { return _inet_addr; }
	const BWNumber http_status_code() const { return _http_status_code; }
	
	const BWString host( const BWString & h );
	const BWNumber port( const BWNumber & p );

	const BWString get_host() const { return _host; }
	const BWNumber get_port() const { return  _port; }

	BWString fetch( const BWString & uri );
};

#endif /* defined(__BWLib__BWWebGet__) */
