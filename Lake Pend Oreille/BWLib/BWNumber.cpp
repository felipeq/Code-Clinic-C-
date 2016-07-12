//  BWNumber.cpp
//  A simple number library
//  by Bill Weinman <http://bw.org/>
//  Copyright (c) 2014 The BearHeart Group LLC
//

#include "BWNumber.h"

#pragma mark - constructors

BWNumber::BWNumber( const int & n ) {
	_lnum = n;
	_dnum = double(_lnum = n);
}

BWNumber::BWNumber( const long & n ) {
	_lnum = n;
	_dnum = double(_lnum = n);
}

BWNumber::BWNumber( const double & d ) {
	_lnum = int(_dnum = d);
	_dnum = d;
}

BWNumber::BWNumber( const char * s ) {
	_lnum = atol(s);
	_dnum = atof(s);
}

BWNumber::BWNumber( const BWNumber & bwn ) {
	_lnum = bwn._lnum;
	_dnum = bwn._dnum;
}

BWNumber::~BWNumber() {
	_dnum = _lnum = 0;
}

#pragma mark - utilities

void BWNumber::reset() {
	_lnum = 0L;
	_dnum = 0.0;
}

const char * BWNumber::c_str_long() {
	snprintf(_buffer, __BWNumber_BUFSIZE, "%ld", _lnum);
	return _buffer;
}

const char * BWNumber::c_str_double() {
	snprintf(_buffer, __BWNumber_BUFSIZE, "%lf", _dnum);
	return _buffer;
}

#pragma mark - arithmetic

BWNumber & BWNumber::add( const BWNumber & n ) {
	_lnum += n._lnum;
	_dnum += n._dnum;
	return *this;
}

BWNumber & BWNumber::sub( const BWNumber & n ) {
	_lnum -= n._lnum;
	_dnum -= n._dnum;
	return *this;
}

BWNumber & BWNumber::div( const BWNumber & n ) {
	_lnum /= n._lnum;
	_dnum /= n._dnum;
	return *this;
}

BWNumber & BWNumber::mul( const BWNumber & n ) {
	_lnum *= n._lnum;
	_dnum *= n._dnum;
	return *this;
}

#pragma mark - assignment overloads

BWNumber & BWNumber::operator = ( const BWNumber & n) {
	_lnum = n._lnum;
	_dnum = n._dnum;
	return *this;
}

BWNumber & BWNumber::operator = ( const long & n) {
	_lnum = int(n);
	_dnum = (double) n;
	return *this;
}

BWNumber & BWNumber::operator = ( const double & n) {
	_lnum = (long int) n;
	_dnum = double(n);
	return *this;
}

#pragma mark - operators

BWNumber & BWNumber::operator += ( const BWNumber & n ) {
	return this->add(n);
}

BWNumber & BWNumber::operator -= ( const BWNumber & n ) {
	return this->sub(n);
}

BWNumber & BWNumber::operator /= ( const BWNumber & n ) {
	return this->div(n);
}

BWNumber & BWNumber::operator *= ( const BWNumber & n ) {
	return this->mul(n);
}

#pragma mark - comparison operators

bool BWNumber::operator == ( const BWNumber & rhs ) const {
	if( ( _lnum == rhs._lnum ) && ( _dnum == rhs._dnum ) ) return true;
	else return false;
}

bool BWNumber::operator != ( const BWNumber & rhs ) const {
	if( ( _lnum != rhs._lnum ) && ( _dnum != rhs._dnum ) ) return true;
	else return false;
}

bool BWNumber::operator > ( const BWNumber & rhs ) const {
	if( ( _lnum > rhs._lnum ) && ( _dnum > rhs._dnum ) ) return true;
	else return false;
}

bool BWNumber::operator < ( const BWNumber & rhs ) const {
	if( ( _lnum < rhs._lnum ) && ( _dnum < rhs._dnum ) ) return true;
	else return false;
}

bool BWNumber::operator >= ( const BWNumber & rhs ) const {
	if( ( _lnum >= rhs._lnum ) && ( _dnum >= rhs._dnum ) ) return true;
	else return false;
}

bool BWNumber::operator <= ( const BWNumber & rhs ) const {
	if( ( _lnum <= rhs._lnum ) && ( _dnum <= rhs._dnum ) ) return true;
	else return false;
}

#pragma mark - increment/decrement operators

BWNumber & BWNumber::operator ++ () {
	_lnum += 1;
	_dnum += 1.0;
	return *this;
}

BWNumber BWNumber::operator ++ (int) {
	BWNumber temp = *this;
	++(*this);
	return temp;
}

BWNumber & BWNumber::operator -- () {
	_lnum -= 1;
	_dnum -= 1.0;
	return *this;
}

BWNumber BWNumber::operator -- (int) {
	BWNumber temp = *this;
	--(*this);
	return temp;
}

#pragma mark - conversion operators

BWNumber::operator int() const {
	return (int) _lnum;
}

BWNumber::operator long() const {
	return _lnum;
}

BWNumber::operator double() const {
	return _dnum;
}

//BWNumber::operator BWString() const {
//	
//}

#pragma mark - binary operators

BWNumber operator + ( const BWNumber & lhs, const BWNumber & rhs ) {
	BWNumber rn = lhs;
	rn += rhs;
	return rn;
}

BWNumber operator - ( const BWNumber & lhs, const BWNumber & rhs ) {
	BWNumber rn = lhs;
	rn -= rhs;
	return rn;
}

BWNumber operator / ( const BWNumber & lhs, const BWNumber & rhs ) {
	BWNumber rn = lhs;
	rn /= rhs;
	return rn;
}

BWNumber operator * ( const BWNumber & lhs, const BWNumber & rhs ) {
	BWNumber rn = lhs;
	rn *= rhs;
	return rn;
}

