//  ImageMeta.cpp - for lynda.com course C++ Code Clinic
//  by Bill Weinman <http://bw.org/>
//  Copyright (c) 2014 The BearHeart Group LLC
//  requires exiv2

#include "ImageMeta.h"

// constructor with filename
ImageMeta::ImageMeta(const char * filename)
{
    if(!filename) return;
    _filename = filename;

    try {
        _image = Exiv2::ImageFactory::open(filename);
        if(_image.get() == 0) return;
        _image->readMetadata();
        _exif_data_p = &_image->exifData();
        _iptc_data_p = &_image->iptcData();
		_xmp_data_p = &_image->xmpData();
		
        if(_exif_data_p->empty()) _exif_data_p = nullptr;
        if(_iptc_data_p->empty()) _iptc_data_p = nullptr;
        if(_xmp_data_p->empty()) _xmp_data_p = nullptr;
		
        if(_exif_data_p || _iptc_data_p || _xmp_data_p) find_metadata();
        if(_metadata.size() > 0) find_caption();
    } catch (Exiv2::AnyError& e) {
        _err_code = e.code();
        _err_str = Exiv2::strError().c_str();
    }
}

// copy constructor
ImageMeta::ImageMeta( const ImageMeta & img )
{
    _filename = img._filename;
    _caption = img._caption;
    _iptc_data_p = img._iptc_data_p;
    _exif_data_p = img._exif_data_p;
	_metadata = img._metadata;
    _err_code = img._err_code;
    _err_str = img._err_str;
}

ImageMeta::~ImageMeta()
{
    _filename = std::string();
    _caption = std::string();
    _iptc_data_p = nullptr;
    _exif_data_p = nullptr;
    _err_code = 0;
    _err_str = nullptr;
}

// local utilities

void ImageMeta::find_metadata()
{
    if(_exif_data_p) {
        for ( auto m : * _exif_data_p ) {
            _metadata.insert({ m.key(), m.value().toString() });
        }
    }
    if (_iptc_data_p) {
        for ( auto m : * _iptc_data_p ) {
            _metadata.insert({ m.key(), m.value().toString() });
        }
    }
    if (_xmp_data_p) {
        for ( auto m : * _xmp_data_p ) {
            _metadata.insert({ m.key(), m.value().toString() });
        }
    }
}

void ImageMeta::find_caption()
{
	static std::string langstr = "lang=";
    
	for ( auto p : _metadata ) {
		std::string const & k = p.first;
        std::string const & v = p.second;
        for ( std::string cap : _caption_strings ) {
			if(k.length() < cap.length()) continue;

			// compare lowercase
			std::string kl = k;
			for( char &c : kl ) c = tolower(c);

            // Do we have a caption? 
		    if(kl.compare(kl.length() - cap.length(), cap.length(), cap) == 0) {
				// XMP data may have "lang=..." in front of string
                if((v.length() > langstr.length()) &&  v.compare(0, langstr.length(), langstr) == 0) {
                    std::string vt = v;
                    size_t pos = vt.find_first_of(' ') + 1;
                    if(vt.length() > pos) vt = vt.substr(pos);
                    _caption = vt;
                } else {
                    _caption = v;
                }
                return;
            }
        }

	}
}

