//  BWWebGet.cpp
//  Get Objects from Web via HTTP
//  by Bill Weinman <http://bw.org/>
//  Copyright (c) 2014 The BearHeart Group LLC
//
//	2014-05-09 -- support for Windows sockets (Winsock)
//

#include "BWWebGet.h"

BWWebGet::BWWebGet( const BWString & host ) : _host(host), _port(80) {
	_connect();
}

BWWebGet::BWWebGet( const BWString & host, const BWNumber & port ) : _host(host), _port(port) {
	_connect();
}

BWWebGet::~BWWebGet() {
	reset();
}

void BWWebGet::socketclose() {
#ifdef _MSC_VER
	closesocket(_socket);
#else
	close(_socket);
#endif
}

void BWWebGet::reset() {
	_host.reset();
	_port.reset();
	if(_have_socket) {
		socketclose();
		_have_socket = false;
		_socket = 0;
	}
	
}

#ifdef _MSC_VER
void BWWebGet::_set_error() {
	_errno = WSAGetLastError();
	strerror_s(__errstr, _STRERROR_SIZE, _errno);
	_errstr = __errstr;
}
#else
void BWWebGet::_set_error() {
	_errno = errno;
	_errstr = strerror(_errno);
}
#endif

void BWWebGet::_reset_error() {
	_errno = 0;
	_errstr = nullptr;
}


void BWWebGet::_set_http_status_code () {
	static const char * http = "HTTP";
	if(!_header.have_value()) return;
	BWString http_status_line = _header.split(_crlf, 2)[0];
	if(!http_status_line.have_value()) return;
	std::vector<BWString> parts = http_status_line.split(" ");
	if(strncmp(parts[0].c_str(), http, strlen(http)) != 0) return;
	_http_status_code = BWNumber(atoi(parts[1]));
}

const BWString BWWebGet::host( const BWString & h ) {
	_host = h;
	return _host;
}

const BWNumber BWWebGet::port( const BWNumber & p ) {
	_port = p;
	return _port;
}

// get socket address, IPv4 or IPv6:
void * BWWebGet::_get_in_addr(struct sockaddr * sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
	
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

#ifdef _MSC_VER

// Winsock sockets

// set socket blocking mode (true = blocking, false = non-blocking)
void BWWebGet::_set_blocking( bool f ) {
	unsigned long mode = f ? 0 : 1;
    ioctlsocket(_socket, FIONBIO, &mode);
}

int BWWebGet::_connect() {
	_reset_error();
	memset(_inet_addr, 0, INET6_ADDRSTRLEN);

	if( !( _host.have_value() && (int) _port ) ) return -1;

	int rv;
	struct addrinfo hints, *servinfo, *p;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    int wsa_err = WSAStartup(_wsaVersionRequested, &_wsa_data);
    if(wsa_err != 0) {
        printf("WSAStartup failed with error: %d\n", wsa_err);
        return _bwwg_failure;
    }
    
	if((rv = getaddrinfo(_host.c_str(), _port.c_str_long(), &hints, &servinfo)) != 0) {
		_errno = WSAGetLastError();
		_errstr = gai_strerror(_errno);
        return _bwwg_failure;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((_socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            continue;	// couldn't create the socket, try the next one
        }
		
        if (::connect(_socket, p->ai_addr, p->ai_addrlen) == -1) {
            socketclose();
            continue;	// couldn't connect, try the next one
        }
		
        break;	// connected
    }
	
    if (p == NULL) {
        return _bwwg_failure;
    }
	
    inet_ntop(p->ai_family, _get_in_addr((struct sockaddr *)p->ai_addr), _inet_addr, INET6_ADDRSTRLEN);
    freeaddrinfo(servinfo); // free it when done
	
	_have_socket = true;
	return true;
}

#else

// POSIX sockets

// set socket blocking mode (true = blocking, false = non-blocking)
void BWWebGet::_set_blocking( bool f ) {
	int flags = 0;
	if ((flags = fcntl(_socket, F_GETFL, 0)) < 0) {
		_set_error();
		return;
	}
	if (f) {
		fcntl(_socket, F_SETFL, flags & (~O_NONBLOCK));
	} else {
		fcntl(_socket, F_SETFL, flags | O_NONBLOCK);
	}
}

int BWWebGet::_connect() {
	_reset_error();
	memset(_inet_addr, 0, INET6_ADDRSTRLEN);

	if( !( _host.have_value() && (int) _port ) ) return -1;

	int rv;
	struct addrinfo hints, *servinfo, *p;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if((rv = getaddrinfo(_host.c_str(), _port.c_str_long(), &hints, &servinfo)) != 0) {
		_errno = rv;
		_errstr = gai_strerror(_errno);
        return _bwwg_failure;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((_socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            continue;	// couldn't create the socket, try the next one
        }
		
        if (::connect(_socket, p->ai_addr, p->ai_addrlen) == -1) {
            socketclose();
            continue;	// couldn't connect, try the next one
        }
		
        break;	// connected
    }
	
    if (p == NULL) {
        return _bwwg_failure;
    }
	
    inet_ntop(p->ai_family, _get_in_addr((struct sockaddr *)p->ai_addr), _inet_addr, INET6_ADDRSTRLEN);
    freeaddrinfo(servinfo); // free it when done
	
	_have_socket = true;
	return true;
}

#endif

BWString BWWebGet::fetch( const BWString & uri ) {
	BWString request = "GET " + uri + " HTTP/1.0" + _newline;

	BWString ua_string;
	ua_string.format("User-agent: %s/%s", _bwwg_ua_string, __BWNumber_VERSION);
	
	request += "User-agent: " + ua_string + _crlf;
	request += "Host: " + _host + _crlf + _crlf;

	_send(request.c_str());
	_receive();

	if(_buffer) {	// split header & body
		std::vector<BWString> rv = BWString(_buffer).split(_crlf + _crlf, 2);
		_header = rv[0];
		_body = rv[1];
		_set_http_status_code();
		return _body;
	} else {
		return BWString();
	}
}

void BWWebGet::_send( const char * msg ) {
	_reset_error();
	_set_blocking(true);
	size_t rc = ::send(_socket, msg, strnlen(msg, _bwwg_max_buf), 0);
	if (rc == _bwwg_failure) {
		_set_error();
	}
}

// non-blocking receive with timeout
void BWWebGet::_receive() {
	_reset_error();
	int rc = 0;
	size_t numbytes = 0;
	char * buf = _buffer;
	size_t max = _bwwg_max_buf;
	memset(_buffer, 0, _bwwg_max_buf);
	
	if(!_have_socket) _connect();

	_set_blocking(false);
    struct timeval tv;
    fd_set readfds;
	
    tv.tv_sec = _bwwg_timeout_secs;
    tv.tv_usec = 0;
	
    FD_ZERO(&readfds);
    FD_SET(_socket, &readfds);

	for(;;) {
		rc = select(_socket + 1, &readfds, NULL, NULL, &tv);
		if(rc < 0) {
			_set_error();
			return;
		} else if (rc == 0) {
			return; // timeout
		} else {
			numbytes = recvfrom(_socket, buf, max  - 1, 0, nullptr, 0);

			if (numbytes == 0) {
				return;
			} else if(numbytes == -1) {
				_set_error();
				return;
			} else {
				buf += numbytes;
				max -= numbytes;
			}
		}
	}
}
