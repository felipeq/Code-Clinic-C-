//  BWDB.h
//  A simple database wrapper for SQLite
//  by Bill Weinman <http://bw.org/>
//  Copyright (c) 2014 The BearHeart Group LLC
//

#ifndef __BWDB__BWDB__
#define __BWDB__BWDB__

#include <vector>
#include <map>
#include <memory>
#include <cstdlib>
#include <sqlite3.h>
#include "BWString.h"

#define __BWDB_VERSION "1.0.16"

class BWDB {
	typedef std::map<BWString, BWString> row;
	row * rowp = nullptr;

	const char * _filename = nullptr;		// database filename
	sqlite3 * _dbh = nullptr;				// database handle
	sqlite3_stmt * stmt = nullptr;			// sqlite3 statement handle

	BWString _value;						// Simple smart string
	
	bool db_okay;							// true if handle is okay
	
	// rule of three
	BWDB( BWDB & );							// no copy constructor
	BWDB operator = ( BWDB & );				// no assignment operator
	BWDB();									// no default constructor

protected:
	void _finalize();						// statement finalize
	void _set_value( const char * s );		// setter
	const char * _get_value() const ;		// getter
	void _clear_value();					// free
	void _clear_row();

	void get_column(int cc);
public:
	typedef row dbrow;
	static const char * version() { return __BWDB_VERSION; }

	BWDB( const char * fn );				// constructor with filename
	~BWDB();								// destructor
	
	bool status();							// get database status

	const char * filename() const;			// filename getter
	const sqlite3 * dbh() const;			// database handle getter
	void close();

	const char * get_error() const;
	const int error_code() const;
	bool have_error() const;

	long count(const BWString & table_name);
	bool table_exists(const BWString & table_name);
	bool do_query(const char *);
	bool do_query(const BWString &);
	bool do_query(const BWString &, const std::vector<BWString> &);

	BWString get_query_value(const BWString &);
	BWString get_query_value(const BWString &, const std::vector<BWString> &);

	bool prepare_query(const BWString &, const std::vector<BWString> &);
	const dbrow * get_prepared_row();
	BWString get_prepared_value();
};

#endif /* defined(__BWDB__BWDB__) */
