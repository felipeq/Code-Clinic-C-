//  main.cpp
//  for csvweb project
//  Copyright (c) 2014 The BearHeart Group LLC
//

#include "CSV.h"
#include "BWCLS.h"
#include "BWXML.h"
#include "BWString.h"
#include <cstdio>
#include <vector>

#define CSV_WEB_VERSION "1.1.4"

void help(const char * me) {
    printf("usage: %s [--html=filename] csvfile1 csvfile2 ...\n\n", me);
    printf("--help for this message\n\n");
    printf("version %s (CSV/%s)\n", CSV_WEB_VERSION, CSV::version());
}

#define _BUFSIZE (1024 * 10)            // 10K
#define _MAX_HTML_FILE (1024 * 1024)    // 1M

BWString readfile( const BWString & filename ) {
    static char filebuf[_BUFSIZE];
    
    BWString file_content;

    // read htmlfile
    FILE * fh = fopen(filename.c_str(), "r");
    if(!fh) {
        printf("cannot open %s (%s).\n", filename.c_str(), strerror(errno));
        return file_content;
    }

    size_t rc = 0;
    size_t _filelen = 0;

    // read file in _BUFSIZE chunks, accumulat in html_file_string
    while ( (rc = fread(filebuf, sizeof(char), _BUFSIZE - 1, fh)) != 0 ) {
        _filelen += rc;
        if (_filelen >= _MAX_HTML_FILE) {
            printf("%s: file too big\n", filename.c_str());
            return file_content;
        }
        filebuf[rc] = 0;
        file_content += filebuf;
    }

    fclose(fh);
    return file_content;
}

BWString csv_to_html(BWString & filename) {
    CSV csv(filename.c_str());
    if (csv.get_error()) {
        return BWXML().comment(BWString().format("%s: %s", filename.c_str(), csv.get_errstr()));
    }

    BWXML table("table", { { "class", "table1" } });
    BWXML tr("tr");
    BWString tablestr = "\n";
    for (size_t i = 0; i < csv.size(); ++i) {
        BWString rowstr;
        auto row = csv.get_row(i);
        for (size_t col = 0; col < csv.columns(); ++col) {
            BWXML cell(i == 0 ? "th" : "td", {}, row[col].c_str());
            rowstr += cell.container();
        }
        tablestr += tr.container(rowstr) + "\n";
    }
    return table.container(tablestr);
}

int main(int argc, const char * argv[])
{
    BWCLS cls(argc, argv);
    BWString html_file_string;
    bool have_html = false;
    
    if (cls["help"].have_value() || cls.get_values().size() < 2) {
        help(cls.get_values()[0]);
        return 0;
    }

    std::vector<BWString> csv_files;
    std::vector<BWString> cls_values = cls.get_values();

    BWString htmlfile = cls["html"];
    have_html = htmlfile.have_value();

    if(have_html) {
        html_file_string = readfile(htmlfile);
        if (!html_file_string.have_value()) return 0;
    } else {
        puts(BWXML().doctype());
        puts(BWXML().comment("Table example from lynda.com C++ Code Clinic"));
        puts(BWXML().comment(BWString().format("Generator: csvweb/%s BWXML/%s [http://cpp.bw.org/]", CSV_WEB_VERSION, BWXML::version())));
        puts(BWXML().tag("html"));
        puts(BWXML().tag("head"));
        
        puts(BWXML("title").container("Code Clinic Table Example").c_str());
        puts(BWXML("meta", { { "charset", "UTF-8" } }).empty_element().c_str());
        puts(BWXML("link", { { "rel", "stylesheet" }, { "type", "text/css" }, { "href", "main.css" } }).empty_element().c_str());
        puts(BWXML("link", { { "rel", "stylesheet" }, { "type", "text/css" }, { "href", "table.css" } }).empty_element().c_str());
        puts(BWXML().end_tag("head"));
        
        puts(BWXML().tag("body"));
    }

    for (int i = 1; i < cls_values.size(); ++i) {
        BWString csv_filename = cls_values[i];
        BWString table_string = csv_to_html(csv_filename);
        
        if (have_html) {
            BWString search_string = BWString().format("<!-- ##%s## -->", csv_filename.c_str());
            BWString repl_string = BWXML().comment(BWString().format("csvweb/%s BWXML/%s [http://cpp.bw.org/] %s", CSV_WEB_VERSION, BWXML::version(), csv_filename.c_str()));
            repl_string += "\n" + table_string;
            repl_string += "\n" + BWXML().comment(BWString().format("end %s", csv_filename.c_str()));
            html_file_string = html_file_string.replace(search_string, repl_string);
        } else {
            puts(table_string);
        }
    }

    if (have_html) {
        puts(html_file_string);
    } else {
        puts(BWXML().end_tag("body"));
        puts(BWXML().end_tag("html"));
    }

    return 0;
}

