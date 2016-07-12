//  BWLPO.h - for lynda.com course C++ Code Clinic
//  by Bill Weinman <http://bw.org/>
//  Copyright (c) 2014 The BearHeart Group LLC
//  Requires BWDB, BWString

#ifndef __BWLPO__
#define __BWLPO__

#include <cstdio>
#include <vector>
#include "BWLib/BWDB.h"
#include "BWLib/BWString.h"
#include "BWLib/BWWebGet.h"

#ifdef _MSC_VER
#include "bw_msposix.h"
#endif

#define __BWLPO_VERSION "1.1.2"

// constant strings
static const char *_kairtemp = "airtemp";
static const char *_kbarpressure = "barpressure";
static const char *_kwindspeed = "windspeed";

// url constants for navy LPO data
static const char *_url_base = "http://lpo.dt.navy.mil/data/DM/";
static const std::map<BWString, BWString> _url_xlat = {
        {_kairtemp,     "Air_Temp"},
        {_kbarpressure, "Barometric_Press"},
        {_kwindspeed,   "Wind_Speed"}
};

static unsigned char _iDATE = 0;
static unsigned char _iTIME = 1;
static unsigned char _iDATA = 2;

static const char *_klpo_name = "lpo";
static const char *_lpo_sql =
        "CREATE TABLE lpo ( id INTEGER PRIMARY KEY, type TEXT, stamp TEXT NOT NULL, value INTEGER );"
                "CREATE UNIQUE INDEX typestamp ON lpo(type, stamp);";

static const char *_json_format =
        "{ \"%s\": {\n"
                "  \"airTemperature\": { \"mean\": %lf, \"median\": %lf },\n"
                "  \"barometricPressure\": { \"mean\": %lf, \"median\": %lf },\n"
                "  \"windSpeed\": { \"mean\": %lf, \"median\": %lf }\n"
                "} }\n";

static const char *_json_pair_format = "{ \"%s\": \"%s\" }\n";

// e.g., lpo.dt.navy.mil /data/DM/2014/2014_02_27/Wind_Gust
static const char *_lpo_host = "lpo.dt.navy.mil";
static const char *_lpo_uri_base = "/data/DM";

class BWLPO
{
    BWDB *db;
    const BWString _lpo_name = _klpo_name;

    struct numval
    {
        long ivalue;
        double dvalue;
        bool valid;
    };

    BWLPO();        // disallow default constructor

    bool _web_get(const BWString &date, const BWString &type, const BWString &pathstring);

public:
    typedef struct numval lporesult;

    static const char *version()
    { return __BWLPO_VERSION; }

    BWLPO(BWDB *bwdb);

    const BWDB *get_db() const;

    const char *get_db_error() const;

    bool add_line(const BWString &line, const BWString &type);

    bool fetch(const BWString &date);

    long count(const BWString &type) const;

    long count(const BWString &type, const BWString &date) const;

    lporesult get_mean(const BWString &type, const BWString &date) const;

    lporesult get_median(const BWString &type, const BWString &date) const;

    BWString json(const BWString &date);

    BWString json_pair(const BWString &lhs, const BWString &rhs);
};

#endif /* defined(__BWLPO__) */
