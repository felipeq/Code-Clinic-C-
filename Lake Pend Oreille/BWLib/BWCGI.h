//  BWCGI.h
//  Simple CGI Interface
//  by Bill Weinman <http://bw.org/>
//  Copyright (c) 2014 The BearHeart Group LLC
//

#ifndef __BWLib__BWCGI__
#define __BWLib__BWCGI__

// MSVS -- turn off _s warnings
#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <cstdlib>
#include <map>
#include "BWString.h"

#define __BWCGI_VERSION "1.0.5"
static const char * _bwcgi_kenv_query_string = "QUERY_STRING";
static const char * _bwcgi_kenv_document_root = "DOCUMENT_ROOT";
static const char * _bwcgi_kenv_gateway_interface = "GATEWAY_INTERFACE";
static const char * _bwcgi_kenv_reqeust_method = "REQUEST_METHOD";
static const char * _bwcgi_kenv_path_info = "PATH_INFO";
static const char * _bwcgi_kenv_script_name = "SCRIPT_NAME";
static const char * _bwcgi_kenv_server_software = "SERVER_SOFTWARE";
static const char * _bwcgi_kenv_content_type = "CONTENT_TYPE";
static const char * _bwcgi_kenv_cookie = "HTTP_COOKIE";
static const char * _bwcgi_kenv_user_agent = "HTTP_USER_AGENT";

class BWCGI {
	const BWString _crlf = "\r\n";
	const BWString _newline = "\n";
	const BWString _content_type = "Content-type";

	BWString _status;

	typedef std::map<BWString, BWString> _envmap;
	_envmap _env;
	_envmap _qmap;

	void process_query_string();
	void message(const char * format, ...);
	
public:
	typedef _envmap envmap;
	static const char * version() { return __BWCGI_VERSION; }

	BWCGI();

	void disp_page( const BWString & type, const BWString & s );
	const BWString getvar( const BWString & ) const;
	const BWString getq( const BWString & ) const;
	envmap & env();
	envmap & qmap();

	BWString status() const;

	bool have_cgi() const;
	bool have_query_string() const;
};

#endif /* defined(__BWLib__BWCGI__) */
