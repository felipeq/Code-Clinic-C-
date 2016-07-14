//  BWCLS
//  Simple Command Line Switch Processor
//  by Bill Weinman <http://bw.org/>
//  Copyright (c) 2014 The BearHeart Group LLC
//

#include "BWCLS.h"

BWCLS::BWCLS( const unsigned int argc, const char ** argv ) : _argc(argc), _argv(argv) {
    process_args();
}

void BWCLS::process_args() {
    const char * arg = nullptr;
    for ( int i = 0; i < _argc; ++i ) {
        arg = _argv[i];
        if (*arg == '-') {
            add_switch(arg);
        } else {
            add_arg(arg);
        }
	}

}

void BWCLS::add_switch(const char * c_arg) {
    static const char equal = '=';
    static const char dash = '-';

    // make sure the string isn't out of range
    if(strnlen(c_arg, __BWCLS_MAXSTRLEN) >= __BWCLS_MAXSTRLEN) return;

    // skip leading dashes
    while(*c_arg == dash) {
        if(*++c_arg == 0) return;
    }

    BWString switch_str(c_arg);
    if (switch_str.char_find(equal) >= 0) {
        std::vector<BWString> parts = switch_str.split(equal);
        _switches.insert({ parts[0], parts[1] });
    } else {
        _switches.insert({ switch_str, switch_str });
    }
}

void BWCLS::add_arg(const char * c_arg) {
    BWString arg_str(c_arg);
    _values.push_back(arg_str);
}

BWString BWCLS::switch_value( const BWString & key ) const {
    BWString v;
    if(_switches.find(key) != _switches.end()) v = _switches.at(key);
    return v;
}

BWString BWCLS::operator [] ( const BWString & key ) const {
    return switch_value(key);
}
