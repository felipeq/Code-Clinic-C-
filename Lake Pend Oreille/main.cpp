//  lpo - main.cpp
//  by Bill Weinman <http://bw.org/>
//  Copyright (c) 2014 The BearHeart Group LLC
//  Requires BWLPO, BWDB, BWString
//

#include <cstdio>
#include "BWLib/BWDB.h"
#include "BWLPO.h"
#include "BWLib/BWCGI.h"

// update this with the location of your database file
// make sure this process has write permission for both the
// file and it's directory
static const char * dbfile = "/Users/billw/sqlite3_data/lpo.db";

static bool cgi_flag = false;

void message ( const char * format, ... );
static BWString _outstr;
static BWString _content_type = "application/json";

int main(int argc, const char * argv[])
{
    BWDB *db = new BWDB(dbfile);
    BWCGI cgi;
    BWLPO lpo(db);

    cgi_flag = cgi.have_cgi();

    if (cgi_flag) { // CGI mode

        BWString date = cgi.getq("date");
        if (!date.have_value()) {
            date = "2014-01-01";
        }
        bool rflag = lpo.fetch(date);
        if (rflag) {
            message("%s\n", lpo.json(date).c_str());
        } else {
            int ec = db->error_code();
            if(ec > 0 && ec < 100) {
                BWString errorstr = "could not fetch data: ";
                errorstr += db->get_error();
                message(lpo.json_pair("error", errorstr).c_str());
            } else {
                message(lpo.json_pair(date, "no data.").c_str());
            }
        }
        cgi.disp_page(_content_type, _outstr);

    } else {    // not CGI

        const char * date;

        if (argc > 1) {
            date = argv[1];
        } else {
            date = "2014-04-20";
            message("no date provided, using default (%s)\n", date);
        }

        bool rflag = lpo.fetch(date);
        if (rflag) {
            message("%s:\n", date);
            BWLPO::lporesult air_mean = lpo.get_mean(_kairtemp, date);
            BWLPO::lporesult air_median = lpo.get_median(_kairtemp, date);
            BWLPO::lporesult bar_mean = lpo.get_mean(_kbarpressure, date);
            BWLPO::lporesult bar_median = lpo.get_median(_kbarpressure, date);
            BWLPO::lporesult wind_mean = lpo.get_mean(_kwindspeed, date);
            BWLPO::lporesult wind_median = lpo.get_median(_kwindspeed, date);
            message("air temperature mean: %lf median: %lf\n", air_mean.dvalue, air_median.dvalue);
            message("barametric pressure mean: %lf median: %lf\n", bar_mean.dvalue, bar_median.dvalue);
            message("wind speed mean: %lf median: %lf\n", wind_mean.dvalue, wind_median.dvalue);
        } else {
            int ec = db->error_code();
            if(ec > 0 && ec < 100) message("could not fetch data: %s\n", db->get_error());
            else message("no data.\n");
        }

    }   // if CGI / not CGI

    if(db) db->close();
    return 0;
}

void message ( const char * format, ... ) {
    char * buffer;
    
    va_list args;
    va_start(args, format);
    
    vasprintf(&buffer, format, args);

    if (cgi_flag) {
        _outstr += buffer;
    } else {
        std::fputs(buffer, stdout);
    }

    free(buffer);
}

