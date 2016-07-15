//  CSV.h
//  CSV routines for csvweb project
//  by Bill Weinman <http://bw.org/>
//  Copyright (c) 2014 The BearHeart Group LLC
//

#ifndef __CSV__
#define __CSV__

#ifdef _MSC_VER
// disable _s warnings
#define _CRT_SECURE_NO_WARNINGS
// disable pragma warnings
#pragma warning( disable : 4068 )
#endif

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <string>
#include <cerrno>

#define _CSV_VERSION "1.0.5"

#define _CSV_MAXSTR 4096
#define _CSV_MAXFILE (_CSV_MAXSTR * 1024)

#define _LINE_ENDING_LF     1
#define _LINE_ENDING_CR     2
#define _LINE_ENDING_CRLF   3

#define _CSV_ERR_NOERROR        0
#define _CSV_ERR_MEMORY         1
#define _CSV_ERR_FILE           2
#define _CSV_ERR_LINE_ENDINGS   3
#define _CSV_ERR_COLUMNS        4

static const char * _csv_errstr[] = {
    "no error",
    "insufficient memory",
    "cannot open file",
    "inconsistent line endings",
    "inconsistent columns"
};

static const char * _csv_whitespace = "\n\r\t\x20";
static const char _csv_escape = '\\';

class CSV {
    const std::string empty_string;

    std::vector<const char *> _lines;
    int _errno = 0;
    char _strbuf[_CSV_MAXSTR];
    char * _filebuf = nullptr;
    size_t _filelen = 0;
    size_t _line_count = 0;
    int _line_no = 0;
    std::vector<int> _cr_list;
    std::vector<int> _lf_list;

    typedef std::vector<std::vector<std::string>> _matrix_type;
    _matrix_type _matrix;
    
    int _line_endings = 0;
    size_t _num_columns = 0;

    const char * _strtrim( const char * str );
    std::vector<int> _find_in_string( const char * haystack, const char needle );
    bool _strhaschar( const char * haystack, const char needle );
    const char * _nextline();
    void _matrix_add( char * line );
    char * _alloc_filebuf();
    void _free_filebuf();

    CSV();
    CSV( CSV & );
public:
	static const char * version() { return _CSV_VERSION; }
    typedef std::vector<std::string> row_type;

    CSV( const char * filename );
    ~CSV();

    size_t size() { return _matrix.size(); }
    size_t columns() { return _num_columns; }
    int get_error() const { return _errno; }
    const char * get_errstr() const { return _csv_errstr[_errno]; }
    row_type get_row( size_t row ) { return _matrix[row]; }
    const std::string & get_value( size_t row, size_t column );
};

#endif /* defined(__CSV__) */
