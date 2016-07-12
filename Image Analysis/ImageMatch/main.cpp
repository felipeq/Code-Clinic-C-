//
//  main.cpp
//  imgmatch
//
//  Created by Bill Weinman on 2014-04-24.
//  Copyright (c) 2014 Bill Weinman. All rights reserved.
//

#include <cstdio>
#include <cerrno>
#include <cstring>
#include <dirent.h>
#include <sys/types.h>

#include "BWString.h"
#include "JPEGData.h"

BWString filepath = "/Users/billw/Desktop/Images";
BWString ref_file = "78771293.jpg";

int main(int argc, const char * argv[])
{
	BWString filename;
	DIR * dir = nullptr;

	if (filepath.last_char() != '/') {
		filepath += "/";
	}

	JPEGData ref_jpeg = filepath + ref_file;
	if(!ref_jpeg.read_jpeg()) {
	    return 1;
	}

	struct dirent * dirp = nullptr;
	dir = opendir(filepath.c_str());
	if (dir == nullptr) {
		printf("cannot open directory %s (%s)\n", filepath.c_str(), strerror(errno));
		return 1;
	}

	while ((dirp = readdir(dir)) != nullptr) {
		BWString dfilename = dirp->d_name;

		if(*dirp->d_name == '.') continue;
		if(ref_file == dfilename) continue;

		JPEGData * j = new JPEGData(filepath + dfilename);
        if(!j->read_jpeg()) {
            return 1;
        }

		const JPEGData::coords *rc = ref_jpeg.find(j);
		if (rc) {
			printf("--> %s (%d x %d): match! (%d x %d)\n", dfilename.c_str(), j->width(), j->height(), rc->x, rc->y);
		} else {
			printf("%s (%d x %d): no match\n", dfilename.c_str(), j->width(), j->height());
		}
		delete j;
	}

	if(dir) closedir(dir);
	return 0;
}

