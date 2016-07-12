//  BWNumber.cpp
//  A simple number library
//  by Bill Weinman <http://bw.org/>
//  Copyright (c) 2014 The BearHeart Group LLC
//

#ifndef __BWLib__BWNumber__
#define __BWLib__BWNumber__

#ifdef _MSC_VER
// disable _s warnings
#define _CRT_SECURE_NO_WARNINGS
// disable pragma warnings
#pragma warning( disable : 4068 )
#endif

#include <cstdlib>
#include "BWString.h"

#define __BWNumber_VERSION "1.0.9"
#define __BWNumber_BUFSIZE 32

class BWNumber {
	long _lnum;
	double _dnum;

	char _buffer[__BWNumber_BUFSIZE];
	
public:
	static const char * version() { return __BWNumber_VERSION; }
	BWNumber() : _lnum(0L), _dnum(0.0) {};		// default constructor
	BWNumber( const int & n );
	BWNumber( const long & n );
	BWNumber( const double & d );
	BWNumber( const char * s );
	BWNumber( const BWNumber & bwn );		// copy constructor
	~BWNumber();

	void reset();
	const char * c_str_long();
	const char * c_str_double();
	
	BWNumber & add( const BWNumber & n );
	BWNumber & sub( const BWNumber & n );
	BWNumber & div( const BWNumber & n );
	BWNumber & mul( const BWNumber & n );

	BWNumber & operator = ( const BWNumber & );
	BWNumber & operator = ( const long & );
	BWNumber & operator = ( const double & );

	BWNumber & operator += ( const BWNumber & );
	BWNumber & operator -= ( const BWNumber & );
	BWNumber & operator /= ( const BWNumber & );
	BWNumber & operator *= ( const BWNumber & );

	bool operator == ( const BWNumber & ) const;
	bool operator != ( const BWNumber & ) const;
	bool operator > ( const BWNumber & ) const;
	bool operator < ( const BWNumber & ) const;
	bool operator >= ( const BWNumber & ) const;
	bool operator <= ( const BWNumber & ) const;

	BWNumber & operator ++ ();
	BWNumber operator ++ (int);
	BWNumber & operator -- ();
	BWNumber operator -- (int);

	operator int() const;
	operator long() const;
	operator double() const;
	operator BWString() const;
};

BWNumber operator + ( const BWNumber & lhs, const BWNumber & rhs );
BWNumber operator - ( const BWNumber & lhs, const BWNumber & rhs );
BWNumber operator / ( const BWNumber & lhs, const BWNumber & rhs );
BWNumber operator * ( const BWNumber & lhs, const BWNumber & rhs );

#endif /* defined(__BWLib__BWNumber__) */
