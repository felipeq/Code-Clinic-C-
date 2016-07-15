//  BWCLS.h
//  Command Line Switch
//  by Bill Weinman <http://bw.org/>
//  Copyright (c) 2014 The BearHeart Group LLC
//

#ifndef __BWLib__BWCLS__
#define __BWLib__BWCLS__

#include <map>
#include <vector>
#include "BWString.h"

#define __BWCLS_VERSION "1.0.6"
#define __BWCLS_MAXSTRLEN 1024

class BWCLS {
    const unsigned int _argc;
    const char ** _argv;
    std::map<BWString, BWString> _switches;
    std::vector<BWString> _values;

    BWCLS();    // no default constructor
    void process_args();
    void add_switch( const char * arg );
    void add_arg( const char * arg );

public:
    typedef std::map<BWString, BWString> switch_map;
    typedef std::vector<BWString> value_vector;
    BWCLS( const unsigned int argc, const char ** argv );
	static const char * version() { return __BWCLS_VERSION; }
    switch_map & get_switches() { return _switches; }
    value_vector & get_values() { return _values; }
    BWString switch_value( const BWString & key ) const;
    BWString operator []( const BWString & key ) const;
};

#endif /* defined(__BWLib__BWCLS__) */
