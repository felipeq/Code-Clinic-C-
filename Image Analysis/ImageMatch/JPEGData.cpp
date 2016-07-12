//  JPEGData.cpp
//  Read image data from JPEG file
//  by Bill Weinman <http://bw.org/>
//  Copyright (c) 2014 The BearHeart Group LLC
//

#include "JPEGData.h"

JPEGData::~JPEGData() {
	freebuffers();
	_width = _height = _stride = 0;
}

void JPEGData::freebuffers() {
	if(_image) {
		free((void *)_image);
		_image = nullptr;
	}
	if(_scanlines) {
		free((void *)_scanlines);
		_scanlines = nullptr;
	}
}

// fuzzy integer comparison
int JPEGData::fuzzy_cmp( const uint8_t lhs, const uint8_t rhs ) const {
	int diff = lhs - rhs;
	if (diff < -fuzzfactor || diff > fuzzfactor) {
		return diff;
	} else {
		return 0;
	}
}

// find first occurance of rhs in lhs
int JPEGData::row_find( const uint8_t * lhs, const uint16_t llen, const uint8_t * rhs, const uint16_t rlen ) const {
	if(rlen > llen) return NO_MATCH;

	uint16_t difflen = llen - rlen;
	uint16_t max_x = llen - difflen;
	uint16_t x = 0;

	for (x = 0; x < max_x; ++x) {
		if (row_cmp(&lhs[x], rhs, rlen)) {
			return x;
		}
	}

	return false;
}

// return true if rows lhs and rhs are (fuzzy) equal
bool JPEGData::row_cmp( const uint8_t * lhs, const uint8_t * rhs, const uint16_t len ) const {
	if(!(lhs && rhs && len)) {
		return NO_MATCH;
	}

	const uint8_t req_match_count = match_threshold * len / 100;
	uint8_t match_count = 0;
	for (uint16_t x = 0; x < len; ++x) {
		if (fuzzy_cmp(lhs[x], rhs[x]) == 0) {
			++match_count;
		}
		if (match_count >= req_match_count) {
			return true;
		}
	}

	return false;
}

// return true if images lhs and rhs are (fuzzy) equal
bool JPEGData::img_cmp( const uint8_t ** lhs, const uint8_t ** rhs, const uint16_t width, const uint16_t height, const uint16_t x_offset, uint16_t y_offset ) const {

	for (uint16_t i = 0; i < height; ++i) {
		if(!row_cmp(&lhs[i + y_offset][x_offset], rhs[i], width)) {
			return false;
		}
	}
	
	return true;
}

void JPEGData::my_error_exit (j_common_ptr cinfo)
{
	/* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
	JPEGData::my_error_ptr myerr = (my_error_ptr) cinfo->err;
	
	/* Always display the message. */
	/* We could postpone this until after returning, if we chose. */
	(*cinfo->err->output_message) (cinfo);
	
	/* Return control to the setjmp point */
	longjmp(myerr->setjmp_buffer, 1);
}

bool JPEGData::read_jpeg() {

	//  ptr = (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_IMAGE, size);


	FILE * _infile = nullptr;
	struct my_error_mgr jerr;
	
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;

	if (setjmp(jerr.setjmp_buffer)) {
		/* If we get here, the JPEG code has signaled an error.
		 * We need to clean up the JPEG object, close the input file, and return.
		 */
		jpeg_destroy_decompress(&cinfo);
		fclose(_infile);
		return 0;
	}
	/* Now we can initialize the JPEG decompression object. */
	jpeg_create_decompress(&cinfo);
	
	// setup the file for read
	if((_infile = fopen(_filename.c_str(), "rb")) == nullptr) {
		printf("cannot open %s (%s)\n", _filename.c_str(), strerror(errno));
		return false;
	}

	jpeg_stdio_src(&cinfo, _infile);
	(void) jpeg_read_header(&cinfo, true);

	cinfo.scale_num = 1;
	cinfo.scale_denom = scalefactor;
	cinfo.out_color_space = JCS_GRAYSCALE;

	jpeg_start_decompress(&cinfo);

	_width = cinfo.output_width;
	_height = cinfo.output_height;
	_cell_size = cinfo.output_components;
	_stride = _width * cinfo.output_components;

	// allocate space for rows
	freebuffers();
	_image = (uint8_t *) calloc(_stride * _width, _height);
	_scanlines = (const uint8_t **) calloc(_height, sizeof(uint8_t *));

    if(!(_image && _scanlines)) {
        printf("failed to allocate %dMi bytes for image.\n", _stride * _width * _height / 1048576);
        return false;
    }

	for( int i = 0; i < _height; ++i) {
		_scanlines[i] = _image + (i * _stride);
	}

	while (cinfo.output_scanline < cinfo.output_height) {
		int linenum = cinfo.output_scanline;
		jpeg_read_scanlines(&cinfo, (uint8_t **)&_scanlines[linenum], 1);
	}

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	fclose(_infile);
	return true;
}

const uint8_t * JPEGData::row( const int n ) const{
	if(_scanlines) return _scanlines[n];
	else return nullptr;
}

// overload for pointer operand
const JPEGData::coords * JPEGData::find( const JPEGData * rhs ) const {
	return find(*rhs);
}

const JPEGData::coords * JPEGData::find( const JPEGData & rhs ) const {
	static JPEGData::coords rc = { 0, 0 };
	uint16_t lhs_width = _width * _cell_size;
	uint16_t lhs_height = _height * _cell_size;
	uint16_t rhs_width = rhs.width() * _cell_size;
	uint16_t rhs_height = rhs.height() * _cell_size;

	if(!(lhs_width && lhs_height)) return nullptr;
	if(!(rhs_width && rhs_height)) return nullptr;

	if (rhs_width > lhs_width || rhs_height > lhs_height) {
		return nullptr;		// rhs cannot be a subset of lhs if it's bigger
	}

	bool have_match = false;
	const uint8_t ** image_lhs = _scanlines;
	const uint8_t ** image_rhs = rhs.scanlines();

	uint16_t diff_w = lhs_width - rhs_width + 1;
	uint16_t diff_h = lhs_height - rhs_height + 1;

	if (diff_w == 0 && diff_h == 0) {
		have_match = img_cmp(image_lhs, image_rhs, rhs_width, rhs_height, 0, 0);
	} else {
		for (uint16_t y = 0; y < diff_h; ++y) {
			for (uint16_t x = 0; x < diff_w; x += _cell_size) {
				rc.x = x; rc.y = y;
				have_match = img_cmp(image_lhs, image_rhs, rhs_width, rhs_height, x, y);
				if (have_match) return &rc;
			}
		}
	}

	if (have_match) return &rc;
	else return nullptr;
}
