//  BWXML
//  Simple XML generator
//  by Bill Weinman <http://bw.org/>
//  Copyright (c) 2014 The BearHeart Group LLC
//

#ifndef __BWXML__
#define __BWXML__

#include "BWString.h"
#include <vector>

#define __BWXML_VERSION "1.0.4"

class BWXML {
    typedef std::vector<std::pair<const char *, const char *>> _attr_t;

    const char * _tag_attr_f = "<%s %s>";
    const char * _tag_noatt_f = "<%s>";
    const char * _tag_empty_f = "<%s %s />";
    const char * _tag_empty_noatt_f = "<%s />";
    const char * _end_tag_f = "</%s>";
    const char * _attribute_f = "%s=\"%s\"";
    const char * _doctype_f = "<!DOCTYPE %s>";
    const char * _comment_f = "<!-- %s -->";

    BWString _name;
    BWString _content;
    _attr_t _attributes;

public:
    typedef _attr_t attr_t;
    BWXML() {}
    BWXML( const char * name ) : _name(name) {}
    BWXML( const char * name, const attr_t & attrs ) : _name(name), _attributes(attrs) {}
    BWXML( const char * name, const attr_t & attrs, const BWString & content ) : _name(name), _attributes(attrs), _content(content) {}
    BWXML( const BWXML & rhs );
    ~BWXML();

	static const char * version() { return __BWXML_VERSION; }
    void set_name( const BWString & name ) { _name = name; };
    void set_attrs( const attr_t & attrs ) { _attributes = attrs; }
    void add_attr( const char * attr_name, const char * attr_value );
    void add_content( const BWString & content ) { _content = content; }

    const BWString tag() const;
    const BWString end_tag() const;
    const BWString tag( const char * name ) const;
    const BWString end_tag( const char * name ) const;
    const BWString empty_element() const;
    const BWString attributes_string() const;
    const attr_t & attributes() const { return _attributes; }
    const BWString container() const;
    const BWString container( const char * content );
    const BWString doctype() const;
    const BWString doctype( const char * typestr ) const;
    const BWString comment( const char * commentstr ) const;
    
    operator const char * ();
    operator const BWString ();
};

#endif /* defined(__BWXML__) */
