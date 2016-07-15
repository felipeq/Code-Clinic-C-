//  main.cpp - for imagesort
//  lynda.com course C++ Code Clinic
//  by Bill Weinman <http://bw.org/>
//  Copyright (c) 2014 The BearHeart Group LLC
//

#include "ImageMeta.h"
#include "BWCLS.h"
#include "BWString.h"
#include <cstdio>
#include <dirent.h>

std::multimap<BWString, ImageMeta> imagemap;

void process_dir( const char * directory )
{
    DIR * dir = nullptr;
    struct dirent * dirp = nullptr;
    if ((dir = opendir(directory)) == nullptr) {
        printf("cannot open directory %s (%s)\n", directory, strerror(errno));
        return;
    }

    while ((dirp = readdir(dir)) != nullptr) {
        bool haveimg = false;
        BWString dfn = dirp->d_name;
        BWString dirpath = directory;
        size_t dfnlen = dfn.length();

        // ignore entries that begin with '.'
        if(dfn[0] == '.') continue;

        // append filename to dirpath
        if(dirpath[dirpath.length() - 1] != '/') dirpath +=  "/";
        dirpath += dfn;

        if(dirp->d_type == DT_DIR) {
            process_dir(dirpath.c_str());
            continue;
        }

        for ( BWString ext : { ".jpg", ".jpeg", ".png" } ) {
            size_t extlen = ext.length();
            if(dfnlen < extlen) continue;
			if(dfn.substr(dfnlen - extlen, extlen) == ext) {
                haveimg = true;
                break;
            }
        }

        if (haveimg) {
            ImageMeta img(dirpath.c_str());

			// lowercase string for case-insensitive sorting
			BWString lower = img.caption().empty() ? dfn.lower() : BWString(img.caption().c_str()).lower();
            imagemap.insert({lower, img});
        }
    }
    
    if(dir) closedir(dir);
}

void print_usage(const char * me)
{
	printf("usage: %s [switches] directory\n\n", me);
	puts("switches:");
	puts("  --help			print this message");
	puts("  --meta			print all metadata");
}

int main(int argc, const char ** argv )
{
    BWCLS cls(argc, argv);
	bool print_meta = cls["meta"].have_value();
	bool help_flag = cls["help"].have_value();
	
    if(help_flag || cls.get_values().size() < 2) {
		print_usage(argv[0]);
        return 0;
    }

    printf("ImageMeta version %s\n", ImageMeta::version());

	BWString dir = cls.get_values()[1];
    process_dir(dir.c_str());
    for ( auto p : imagemap ) {
		ImageMeta &img = p.second;
		BWString filename = img.filename().c_str();
		BWString caption = img.caption().empty() ? p.first.c_str() : img.caption().c_str();
		BWString display_filename = filename.substr(dir.length(), filename.length() - dir.length());
		
		printf("%s (%s)\n", caption.c_str(), display_filename.c_str());

		if (print_meta) {
            for ( auto p : img.metadata() ) {
                printf("%s: %s\n", p.first.c_str(), p.second.c_str());
            }
            printf("\n");
		}
	}
    return 0;
}

