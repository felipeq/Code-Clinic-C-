//  BWString.h
//  A simple smart string class
//  by Bill Weinman <http://bw.org/>
//  Copyright (c) 2014 The BearHeart Group LLC
//

#ifndef __BWLIB__BWString__
#define __BWLIB__BWString__

#ifdef _MSC_VER
// disable _s warnings
#define _CRT_SECURE_NO_WARNINGS
// disable pragma warnings
#pragma warning( disable : 4068 )
#include "bw_msposix.h"
#endif

#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <string>
#include <vector>

#define __BWString__VERSION "1.2.10"
#define __BWString__MAX_LEN 65535

// simple smart C-string
class BWString {
	char * _str = nullptr;
	size_t _str_len = 0;
	
public:
	static const char * version() { return __BWString__VERSION; }
	BWString();								// default constructor
	BWString( const char * s );
	BWString( const BWString & );			// copy constructor
	~BWString();

	const char * alloc_str( size_t sz );	// smart alloc string
	void reset();							// frees the string
	const char * value() const;				// getter (legacy)
	const char * c_str() const;				// getter
	const char * copy_str( const char * );	// alloc & copy

	// operators
	BWString & operator = ( const char * );			// assignment operator
	BWString & operator = ( const BWString & );		// assignment operator
	BWString & operator += ( const char * );			// concatenation operator
	BWString & operator += ( const BWString & rhs );	// concatenation operator

	// comparisons
	bool operator == ( const BWString & ) const;
	bool operator != ( const BWString & ) const;
	bool operator > ( const BWString & ) const;
	bool operator < ( const BWString & ) const;
	bool operator >= ( const BWString & ) const;
	bool operator <= ( const BWString & ) const;

	// conversion operators
	operator const char * () const;				// c-string type
	operator std::string () const;				// c++ string class

	// utility methods
	bool have_value() const;
	size_t length() const;
	BWString & format( const char * format, ... );
	BWString & trim();
	const char & last_char() const;
	const std::vector<BWString> split( const char * match ) const;
	const std::vector<BWString> split( const char * match, int max_split ) const;
	const BWString & char_repl( const char & match, const char & repl );

};

// function overloads
BWString operator + ( const BWString & lhs, const BWString & rhs );

#endif /* defined(__BWLIB__BWString__) */
