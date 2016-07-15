//  BWString.cpp
//  A simple smart string class
//  by Bill Weinman <http://bw.org/>
//  Copyright (c) 2014 The BearHeart Group LLC
//

#include "BWString.h"

#pragma mark - constructors/destructors

BWString::BWString( ) {
	reset();
}

BWString::BWString( const char * s ) {
	copy_str(s);
}

BWString::BWString( const BWString & old ) {
	copy_str(old);
}


BWString::~BWString() {
	reset();
}

#pragma mark - public methods

const char * BWString::alloc_str( size_t sz ) {
	if (_str) reset();
	_str_len = (sz > __BWString__MAX_LEN) ? __BWString__MAX_LEN : sz;
	_str = (char *) calloc(1, _str_len + 1);
	return _str;
}

void BWString::reset() {
	if(_str) free((void *) _str);
	_str = nullptr;
	_str_len = 0;
}

const char * BWString::value() const {
	return _str;
}

const char * BWString::c_str() const {
	return _str;
}

const char * BWString::copy_str( const char * s) {
	if(s) {
		size_t len = strnlen(s, __BWString__MAX_LEN);
		alloc_str(len);
		strncpy((char *)_str, s, len);
		_str_len = len;
	}
	return _str;
}

#pragma mark - operators

BWString & BWString::operator = ( const char * cs) {
	copy_str(cs);
	return *this;
}

BWString & BWString::operator = ( const BWString & cs ) {
	copy_str(cs.c_str());
	return *this;
}

BWString & BWString::operator += ( const char * rhs ) {
	if(rhs) {
		size_t newlen = _str_len + strnlen(rhs, __BWString__MAX_LEN);
		if (newlen > __BWString__MAX_LEN) newlen = __BWString__MAX_LEN;
		char * buf = (char *) calloc(1, newlen + 1);
		if(!buf) return *this;

		if(_str && _str_len) strncpy(buf, _str, _str_len);
		strncpy(buf + _str_len, rhs, newlen - _str_len);
		
		buf[newlen] = '\0';
		copy_str(buf);
		free(buf);
	}
	return *this;
}

BWString & BWString::operator += ( const BWString & rhs ) {
	operator+=(rhs.c_str());
	return *this;
}

#pragma mark - comparison operators

bool BWString::operator == ( const BWString & rhs ) const {
	if( std::strncmp(this->c_str(), rhs.c_str(), __BWString__MAX_LEN) == 0 ) return true;
	else return false;
}

bool BWString::operator != ( const BWString & rhs ) const {
	if( std::strncmp(this->c_str(), rhs.c_str(), __BWString__MAX_LEN) != 0 ) return true;
	else return false;
}

bool BWString::operator > ( const BWString & rhs ) const {
	if( std::strncmp(this->c_str(), rhs.c_str(), __BWString__MAX_LEN) > 0 ) return true;
	else return false;
}

bool BWString::operator < ( const BWString & rhs ) const {
	if( std::strncmp(this->c_str(), rhs.c_str(), __BWString__MAX_LEN) < 0 ) return true;
	else return false;
}

bool BWString::operator >= ( const BWString & rhs ) const {
	if( std::strncmp(this->c_str(), rhs.c_str(), __BWString__MAX_LEN) >= 0 ) return true;
	else return false;
}

bool BWString::operator <= ( const BWString & rhs ) const {
	if( std::strncmp(this->c_str(), rhs.c_str(), __BWString__MAX_LEN) <= 0 ) return true;
	else return false;
}

#pragma mark - conversion operators

BWString::operator const char * () const {
	return c_str();
}

BWString::operator std::string () const {
	return std::string(c_str());
}

#pragma mark - Utility methods

bool BWString::have_value() const {
	if(_str) return true;
	else return false;
}

// string format
BWString & BWString::format( const char * format , ... ) {
	char * buffer;
	
    va_list args;
    va_start(args, format);
	
    vasprintf(&buffer, format, args);
	copy_str(buffer);
	free(buffer);
	return *this;
}

// trim leading and trailing spaces
BWString & BWString::trim() {
	const static char * whitespace = "\x20\x1b\t\r\n\v\b\f\a";

	if(!have_value()) return *this;	// make sure we have a string

	size_t begin = 0;
	size_t end = length() - 1;

	for (begin = 0; begin <= end; ++begin) {
		if (strchr(whitespace, _str[begin]) == nullptr) {
			break;
		}
	}

	for ( ; end > begin; --end) {
		if (strchr(whitespace, _str[end]) == nullptr) {
			break;
		} else {
			_str[end] = '\0';
		}
	}

	if (begin) {
		for (size_t i = 0; _str[i]; ++i) {
			_str[i] = _str[begin++];
		}
	}

	_str_len = strlen(_str);
	return *this;
}

BWString BWString::lower() const {
	BWString rs = *this;
	for (size_t i = 0; rs._str[i]; ++i) {
		rs._str[i] = tolower(rs._str[i]);
	}
	return rs;
}

BWString BWString::upper() const {
	BWString rs = *this;
	for (size_t i = 0; rs._str[i]; ++i) {
		rs._str[i] = toupper(rs._str[i]);
	}
	return rs;
}

const char & BWString::last_char() const {
	return _str[length() - 1];
}

// non-destructive split
const std::vector<BWString> BWString::split(const char match) const {
    const char match_s[2] = { match, 0 };
	return split(match_s, -1);
}

const std::vector<BWString> BWString::split(const char * match) const {
	return split(match, -1);
}

const std::vector<BWString> BWString::split( const char * match, int max_split ) const {
	std::vector<BWString> rv;
	if (length() < 1) return rv;
	size_t match_len = strnlen(match, __BWString__MAX_LEN);
	if(match_len >= __BWString__MAX_LEN) return rv;
	
	char * mi;				// match index
	char * pstr = _str;		// string pointer
	while (( mi = strstr(pstr, match) ) && ( max_split < 0 || --max_split ) ) {
		if(mi != pstr) {
			size_t lhsz = mi - pstr;
			char * cslhs = (char *) malloc(lhsz + 1);
			cslhs[lhsz] = '\0'; // strncpy doesn't terminate it
			rv.emplace_back(strncpy(cslhs, pstr, lhsz)); // calls BWString copy ctor
			pstr += lhsz;
			free(cslhs);
		}
		pstr += match_len;
	}
	
	if (*pstr != '\0') {
		rv.emplace_back(pstr);
	}
	
	return rv;
}

const BWString & BWString::char_repl( const char & match, const char & repl ) {
	for (size_t i = 0; _str[i]; ++i) {
		if(_str[i] == match) _str[i] = repl;
	}
	return *this;
}

long int BWString::char_find( const char & match ) {
	for (size_t i = 0; _str[i]; ++i) {
		if(_str[i] == match) return i;
	}
	return -1;
}

BWString BWString::substr( size_t start, size_t length ) {
	BWString rs;
	char * buf;
	if ((length + 1) > __BWString__MAX_LEN || (start + length) > __BWString__MAX_LEN) return rs;
	if (length > _str_len - start) return rs;
	if (!_str) return rs;

	buf = (char *) calloc(sizeof(char), length + 1);
	memcpy(buf, _str + start, length);
	rs = buf;
	return rs;
}

#pragma mark - function overloads

BWString operator + ( const BWString & lhs, const BWString & rhs ) {
	BWString rs = lhs;
	rs += rhs;
	return rs;
}
