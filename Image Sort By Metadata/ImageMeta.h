//  ImageMeta.h - for lynda.com course C++ Code Clinic
//  by Bill Weinman <http://bw.org/>
//  Copyright (c) 2014 The BearHeart Group LLC
//  requires exiv2

#ifndef __ImageMeta__
#define __ImageMeta__

#include <exiv2/exiv2.hpp>
#include <string>
#include <map>

#define __ImageMeta__VERSION "1.0.6"

// these are searched in order -- all lowercase for comparison
static const char * _caption_strings[] = {
    ".caption",
    ".description",
    ".imagedescription",
    ".title",
	".objectname",
	".documentname"
};

class ImageMeta {
    Exiv2::Image::AutoPtr _image;
    int _err_code = 0;
    const char * _err_str = nullptr;

    Exiv2::IptcData * _iptc_data_p = nullptr;
    Exiv2::ExifData * _exif_data_p = nullptr;
	Exiv2::XmpData  * _xmp_data_p = nullptr;

	std::string _caption = std::string();
    std::string _filename;
    std::map<std::string, std::string> _metadata;

    ImageMeta();    // no default constructor
    void find_caption();
    void find_metadata();
public:
    typedef Exiv2::IptcData iptc_data;
    typedef Exiv2::ExifData exif_data;
    static const char * version() { return __ImageMeta__VERSION; }

    ImageMeta(const char *);            // constructor with filename
    ImageMeta( const ImageMeta & img ); // copy constructor
    ~ImageMeta();                       // destructor (for completeness)

    iptc_data * get_iptc() const { return _iptc_data_p; }
    exif_data * get_exif() const { return _exif_data_p; }
    std::string filename() const { return _filename; }

    std::string caption() const { return _caption; }
    std::map<std::string, std::string> metadata() const { return _metadata; }
};

#endif /* defined(__ImageMeta__) */
