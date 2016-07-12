//  JPEGData.h
//  Read image data from JPEG file
//  by Bill Weinman <http://bw.org/>
//  Copyright (c) 2014 The BearHeart Group LLC
//

#ifndef __subimg__JPEGData__
#define __subimg__JPEGData__

#include <iostream>
#include <csetjmp>
#include <cerrno>
#include <cstring>
#include <cstdint>
#include <jpeglib.h>
#include "BWString.h"

#ifdef _MSC_VER
// disable _s warnings
#define _CRT_SECURE_NO_WARNINGS
#pragma warning( disable : 4996 )
// disable pragma warnings
#pragma warning( disable : 4068 )
#endif

#define __JPEGData_VERSION "1.0.4"

class JPEGData {

	const BWString _filename;
	const static int MAX_SCANLINES = 65535;
	const static int NO_MATCH = -1;

	const static uint8_t scalefactor = 6;	// value: 1-8; denominator, e.g. 1/n
	const static uint8_t fuzzfactor = 12;
	const static uint8_t match_threshold = 90;	// percent of matches required in a row

	const static uint16_t msx = 16;	// min significant x
	const static uint16_t msy = 16;	// min significant y

	const uint8_t ** _scanlines = nullptr;
	const uint8_t * _image = nullptr;

	uint16_t _width = 0;
	uint16_t _height = 0;
	uint8_t _cell_size = 0;
	uint16_t _stride = 0;

	struct _coords { uint16_t x; uint16_t y; };

	// for jpeglib
	struct my_error_mgr {
		struct jpeg_error_mgr pub;
		jmp_buf setjmp_buffer;
	};
	typedef struct my_error_mgr * my_error_ptr;
	struct jpeg_decompress_struct cinfo;
	static void my_error_exit (j_common_ptr cinfo);

	void freebuffers();
	int fuzzy_cmp( const uint8_t lhs, const uint8_t rhs ) const;
	int row_find( const uint8_t * lhs, const uint16_t llen, const uint8_t * rhs, const uint16_t rlen ) const;
	bool row_cmp( const uint8_t * lhs, const uint8_t * rhs, const uint16_t len ) const;
	bool img_cmp( const uint8_t ** lhs, const uint8_t ** rhs, const uint16_t width, const uint16_t height, const uint16_t x_offset, uint16_t y_offset  ) const;

	JPEGData();	// no default constructor
public:
	static const char * version() { return __JPEGData_VERSION; }
	JPEGData( const char * filename ) : _filename(filename) {};
	JPEGData( const BWString & filename ) : _filename(filename.c_str()) {};
	~JPEGData();

	typedef struct _coords coords;

	bool read_jpeg();
	int width() const { return _width; }
	int height() const { return _height; }

	const BWString & filename() const { return _filename; }

	const uint8_t ** scanlines () const { return _scanlines; }
	const uint8_t * row( const int n ) const;
	const coords * find( const JPEGData & rhs ) const;
	const coords * find( const JPEGData * rhs ) const;
};

#endif /* defined(__subimg__JPEGData__) */
