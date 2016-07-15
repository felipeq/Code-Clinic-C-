//  BWXML
//  Simple XML generator
//  by Bill Weinman <http://bw.org/>
//  Copyright (c) 2014 The BearHeart Group LLC
//

#include "BWXML.h"

// copy constructor
BWXML::BWXML( const BWXML & rhs ) {
    _name = rhs._name;
    _attributes = rhs._attributes;
    _content = rhs._content;
}

BWXML::~BWXML() {
    return;
}

#pragma mark - setters

void BWXML::add_attr( const char * attr_name, const char * attr_value ) {
    _attributes.push_back( { attr_name, attr_value } );
}

#pragma mark - getters

const BWString BWXML::tag( const char * name ) const {
    if (_attributes.size()) {
        return BWString().format(_tag_attr_f, name, attributes_string().c_str());
    } else {
        return BWString().format(_tag_noatt_f, name);
    }
}

const BWString BWXML::end_tag( const char * name ) const {
    return BWString().format(_end_tag_f, name);
}

const BWString BWXML::tag() const {
    return tag(_name.c_str());
}

const BWString BWXML::end_tag() const {
    return end_tag(_name.c_str());
}

const BWString BWXML::empty_element() const {
    if (_attributes.size()) {
        return BWString().format(_tag_empty_f, _name.c_str(), attributes_string().c_str());
    } else {
        return BWString().format(_tag_empty_noatt_f, _name.c_str());
    }
}

const BWString BWXML::attributes_string() const {
    BWString attr;

    for ( auto nv : _attributes ) {
        if(attr.size()) attr += " ";
        attr += BWString().format(_attribute_f, nv.first, nv.second );
	}
    return attr;
}

const BWString BWXML::container() const {
    return BWString(tag()) + _content + end_tag();
}

const BWString BWXML::container( const char * content ) {
    add_content(content);
    return container();
}

#pragma mark - html specific

// defaults to htlm
const BWString BWXML::doctype() const {
    return doctype("html");
}

const BWString BWXML::doctype( const char * typestr ) const {
    return BWString().format(_doctype_f, typestr);
}

const BWString BWXML::comment( const char * commentstr ) const {
    return BWString().format(_comment_f, commentstr);
}
