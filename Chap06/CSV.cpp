//  CSV.cpp
//  CSV routines for csvweb project
//  by Bill Weinman <http://bw.org/>
//  Copyright (c) 2014 The BearHeart Group LLC
//

#include "CSV.h"

CSV::CSV( const char * filename ) {
    _alloc_filebuf();
    FILE * fh = fopen(filename, "rb");
    if(!fh) {
        _errno = _CSV_ERR_FILE;
        return;
    }

    // read the file
    size_t rc = 0;
    _filelen = 0;
    while ( (rc = fread(_filebuf, sizeof(char), _CSV_MAXFILE - _filelen, fh)) != 0 ) {
        _filelen += rc;
        if(_filelen >= _CSV_MAXFILE) {
            _filelen = _CSV_MAXFILE - 1;
            break;
        }
    }
    _filebuf[_filelen] = 0;  // terminate

    const char * line = nullptr;
    while ( (line = _nextline()) ) {
        _matrix_add((char *)_strtrim(line));
    }

    fclose(fh);
    _free_filebuf();
}

CSV::~CSV() {
    _free_filebuf();
    return;
}

char * CSV::_alloc_filebuf() {
    _free_filebuf();
    _filebuf = (char *) malloc(_CSV_MAXFILE);
    if(_filebuf == nullptr) {
        _errno = _CSV_ERR_MEMORY;
    }
    return _filebuf;
}

void CSV::_free_filebuf() {
    if(_filebuf) free(_filebuf);
    _filebuf = nullptr;
}

// find char in C string, return true or false
bool CSV::_strhaschar( const char * haystack, const char needle ) {
    for (int i = 0; haystack[i] && (i < _CSV_MAXSTR); ++i) {
        if (haystack[i] == needle) {
            return true;
        }
    }
    return false;
}

// trim whitespace from start and end of string
const char * CSV::_strtrim( const char * str ) {
    size_t start = 0;
    size_t end = 0;
    size_t len = 0;

    if(*str == 0) return str;   // empty string

    // trim front;
    size_t i = 0;
    for ( i =0 ; str[i]; ++i ) {
        if( i >= _CSV_MAXSTR ) break;
        if( !_strhaschar(_csv_whitespace, str[i]) ) break;
	}
    start = i;

    // trim end
    for( i = strnlen(str, _CSV_MAXSTR) - 1; i > 0; --i) {
        if( !_strhaschar(_csv_whitespace, str[i]) ) break;
    }
    end = i;
    len = end >= start ? end - start + 1 : 0;

    strncpy(_strbuf, str + start, len);
    _strbuf[len] = 0;

    return _strbuf;
}

// find (unescaped) characters in string
// return a vector of indexes into string
std::vector<int> CSV::_find_in_string( const char * haystack, const char needle ) {
    std::vector<int> found;
    for ( int i = 0; haystack[i]; ++i ) {
        if (haystack[i] == _csv_escape) {
            ++i;
        } else if (haystack[i] == needle) {
            found.push_back(i);
        }
	}
    return found;
}

// newline agnostic line getter
// supports CR, LF, and CRLF line-endings
// return line (c string) or nulptr when no more lines
const char * CSV::_nextline() {
    if ( _line_no < 0 ) return nullptr;
    if (!_filelen) return nullptr;

    memset(_strbuf, 0, _CSV_MAXSTR);

    // find all the line-endings
    if(!_line_count) {
        // find all the CRs
        _cr_list = _find_in_string(_filebuf, '\r');
        size_t cr_count = _cr_list.size();

        // find all the LFs
        _lf_list = _find_in_string(_filebuf, '\n');
        size_t lf_count = _lf_list.size();

        if( lf_count ) {
            if ( cr_count == lf_count ) _line_endings = _LINE_ENDING_CRLF;
            else if ( cr_count ) {  // inconsistent line endings
                _errno = _CSV_ERR_LINE_ENDINGS;
                return nullptr;
            }
            else _line_endings = _LINE_ENDING_LF;
            _line_count = lf_count;
        } else if ( cr_count) {
            _line_endings = _LINE_ENDING_CR;
            _line_count = cr_count;
        }
    }

    if ( (unsigned)_line_no > _line_count) {
        _line_no = -1;
        return nullptr;
    }

    int line_start = 0;
    int line_end = 0;
    std::vector<int> & vl = ( _line_endings == _LINE_ENDING_LF ) ? _lf_list : _cr_list;

    if ( _line_no == 0 ) {
        line_start = 0;
        line_end = vl[_line_no];
    } else if ( _line_no == _line_count ) {
        line_start = vl[_line_count - 1] + 1;
        line_end = (int) _filelen;
    } else {
        line_start = vl[_line_no - 1] + 1;
        line_end = vl[_line_no];
    }

    if ( ( _line_no > 0 ) && ( _line_endings == _LINE_ENDING_CRLF ) ) ++line_start;
    int line_len = line_end - line_start;
    if ( line_len >= _CSV_MAXSTR ) line_len = _CSV_MAXSTR - 1;

    strncpy(_strbuf, &_filebuf[line_start], line_len);

    if ((unsigned)_line_no > _line_count) _line_no = -1;
    else ++_line_no;
    
    return _strbuf;
}

void CSV::_matrix_add( char * line ) {
    std::vector<int> commas = _find_in_string( line, ',' );
    size_t commas_cols = commas.size();

    if (commas_cols == 0) return;    // empty line
    if (!_num_columns) _num_columns = commas_cols + 1;

    if ( _num_columns != (commas_cols + 1) ) {
        _errno = _CSV_ERR_COLUMNS;
        return;
    }

    std::vector<std::string> row;
    for (unsigned i = 0; i < _num_columns; ++i) {
        if (i < (_num_columns - 1) ) {
            line[commas[i]] = 0;
        }
        int offset = i > 0 ? commas[i - 1] + 1 : 0;
        char * col_start = line + offset;
        row.push_back(col_start); // std::string constructor makes a copy
    }
    _matrix.push_back(row);
}

const std::string & CSV::get_value( size_t row, size_t column ) {
    if ( _matrix.size() > row && _matrix[row].size() > column ) return _matrix[row][column];
    else return empty_string;
}

