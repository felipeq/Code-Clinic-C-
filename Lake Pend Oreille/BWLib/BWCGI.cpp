//  BWCGI
//  Simple CGI Interface
//  by Bill Weinman <http://bw.org/>
//  Copyright (c) 2014 The BearHeart Group LLC
//

#include "BWCGI.h"

BWCGI::BWCGI() {
	for ( BWString k : { _bwcgi_kenv_query_string, _bwcgi_kenv_gateway_interface, _bwcgi_kenv_document_root, _bwcgi_kenv_reqeust_method, _bwcgi_kenv_path_info, _bwcgi_kenv_script_name, _bwcgi_kenv_server_software, _bwcgi_kenv_content_type, _bwcgi_kenv_user_agent, _bwcgi_kenv_cookie } ) {
		char * s = std::getenv(k);
		if(s && *s) _env[k] = s;
	}
	process_query_string();
}

void BWCGI::process_query_string() {
	if(!have_query_string()) return;
	BWString qs = getvar(_bwcgi_kenv_query_string);
	std::vector<BWString> queries = qs.split("&");
	for ( BWString q : queries ) {
		std::vector<BWString> qv = q.split("=");
		if (qv.size() >= 2) {
			BWString k = qv[0];
			BWString v = qv[1];
			if(k.have_value()) {
				_qmap[k] = v.have_value() ? v : "";
			}
		}
	}
}

void BWCGI::message(const char * format, ...) {
    static const size_t max_buf = 1024;
    static char buf[max_buf];

    va_list args;
    va_start(args, format);

	memset(buf, 0, max_buf);
    vsnprintf(buf, max_buf, format, args);
	_status += buf + _newline;
}


BWString BWCGI::status() const {
	return _status;
}

void BWCGI::disp_page( const BWString & type, const BWString & s ) {
	BWString buf = _content_type + ": " + type + _crlf + _crlf + s;
	fputs(buf.c_str(), stdout);
}

const BWString BWCGI::getvar( const BWString & k ) const {
	auto it = _env.find(k);
	if(it == _env.end()) return BWString();
	else return it->second;
}

const BWString BWCGI::getq( const BWString & k ) const {
	auto it = _qmap.find(k);
	if(it == _qmap.end()) return BWString();
	else return it->second;
}

BWCGI::envmap & BWCGI::env() {
	return _env;
}

BWCGI::envmap & BWCGI::qmap() {
	return _qmap;
}

bool BWCGI::have_cgi() const {
	if (getvar(_bwcgi_kenv_gateway_interface).have_value()) return true;
	else return false;
}

bool BWCGI::have_query_string() const {
	if (getvar(_bwcgi_kenv_query_string).have_value()) return true;
	else return false;
}

