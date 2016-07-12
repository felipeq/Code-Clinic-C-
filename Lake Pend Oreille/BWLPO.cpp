//  BWLPO.cpp - for lynda.com course C++ Code Clinic
//  by Bill Weinman <http://bw.org/>
//  Copyright (c) 2014 The BearHeart Group LLC
//  Requires BWDB, BWString

#include "BWLPO.h"

BWLPO::BWLPO( BWDB * bwdb ) : db(bwdb) {
	if (!db->table_exists(_lpo_name)) {
		db->do_query(_lpo_sql);
	}
}

const BWDB * BWLPO::get_db() const {
	return db;
}

const char * BWLPO::get_db_error() const {
	return db->get_error();
}

long BWLPO::count( const BWString & type ) const {
	BWString rs = db->get_query_value("SELECT COUNT(*) FROM " + _lpo_name + " WHERE type = ?" , { type });
	return atoi(rs.c_str());
}

long BWLPO::count( const BWString & type, const BWString & date ) const {
	BWString datelike = date + "%";
	BWString rs = db->get_query_value("SELECT COUNT(*) FROM " + _lpo_name + " WHERE type = ? AND stamp LIKE ?", {type, datelike});

	if(rs.have_value())  {
		return atol(rs.c_str());
	} else {
		return 0;
	}
}

bool BWLPO::add_line( const BWString & line, const BWString & type ) {
	// trim the whitespace
	BWString linecopy = line;
	linecopy.trim();

	// split out the parts
	std::vector<BWString> lineparts = linecopy.split(" ");
	if(lineparts.size() != 3) return false;

	// date has undescores where SQL dates have dashes
	lineparts[_iDATE].char_repl('_', '-');

	BWString SQLdate = lineparts[_iDATE] + " " + lineparts[_iTIME];

	BWString rs = db->get_query_value("SELECT value FROM " + _lpo_name + " WHERE type = ? AND stamp = ?", {type, SQLdate});
	if (rs.have_value()) {
		db->do_query("UPDATE " + _lpo_name + " SET value = ? WHERE type = ? AND stamp = ?", {lineparts[_iDATA], type, SQLdate});
	} else {
		db->do_query("INSERT INTO " + _lpo_name + " (value, type, stamp) VALUES (?, ?, ?)", {lineparts[_iDATA], type, SQLdate});
	}

	if(db->have_error()) return false;
	else return true;
}

bool BWLPO::_web_get( const BWString & date, const BWString & type, const BWString & pathstring ) {
	BWWebGet *web = nullptr;
	bool rflag = true;
	
	std::vector<BWString> date_parts = date.split("-");
	if (date_parts.size() != 3) return false;

	BWString uri;
	uri.format("%s/%s/%s_%s_%s/%s",
			   _lpo_uri_base,
			   date_parts[0].c_str(),
			   date_parts[0].c_str(),
			   date_parts[1].c_str(),
			   date_parts[2].c_str(),
			   pathstring.c_str()
			   );

	if(!web) web = new BWWebGet(_lpo_host);
	
	BWString body = web->fetch(uri);

	if (body.have_value()) {
		std::vector<BWString> lines = body.split("\n");
		for ( BWString l : lines ) {
			if(!add_line(l, type)) {
				rflag = false;
				break;
			}
		}

	}

	if(web) {
		delete web;
		web = nullptr;
	}
	return rflag;
}

bool BWLPO::fetch( const BWString & date ) {
	BWString rs;
	bool rflag = true;
	
	BWString date_next = db->get_query_value("SELECT DATE(?, '+1 DAY');", { date }); // date of next day
	static const BWString q = "SELECT COUNT(*) FROM " + _lpo_name + " WHERE type = ? AND stamp LIKE ? || '%'";

	for ( std::pair<BWString, BWString> p : _url_xlat ) {
		long c1 = 0;
		long c2 = 0;
		const BWString type = p.first;
		const BWString pathstring = p.second;

		rs = db->get_query_value(q, {type, date});
		c1 = rs.have_value() ? atol(rs.c_str()) : 0;
		rs = db->get_query_value(q, {type, date_next});
		c2 = rs.have_value() ? atol(rs.c_str()) : 0;

		if(!c2 || !c1) rflag = _web_get(date, type, pathstring);
		if(rflag && !c2) _web_get(date_next, type, pathstring);

		rs.reset();
		if(!rflag) break;
	}
	return rflag;
}

BWLPO::lporesult BWLPO::get_mean( const BWString & type, const BWString & date ) const {
	lporesult rv = { 0L, 0.0, false };
	BWString date_like = date + "%";
	BWString rs = db->get_query_value("SELECT AVG(value) FROM " + _lpo_name + " WHERE type = ? AND stamp LIKE ?", {type, date_like});
	
	if(rs.have_value()) {
		rv.ivalue = atol(rs.c_str());
		rv.dvalue = atof(rs.c_str());
		rv.valid = true;
		return rv;
	} else {
		return rv;
	}
}

BWLPO::lporesult BWLPO::get_median( const BWString & type, const BWString & date ) const {
	lporesult rv;
	long nrecs = count(type, date);

	if (nrecs < 3) {	// 0, 1, or 2 - mean is same as median
		return BWLPO::get_mean(type, date);
	}
	
	const static int n_max_size = 16;
	char nstring[n_max_size];
	BWString date_like = date + "%";
	
	rv = { 0L, 0.0, false };
	if(nrecs % 2) {		// odd - get the middle value
		snprintf(nstring, n_max_size, "%ld", nrecs / 2 );
		BWString rs = db->get_query_value("SELECT value FROM " + _lpo_name + " WHERE type = ? AND stamp LIKE ? ORDER BY value LIMIT 1 OFFSET ?", {type, date_like, nstring});
		if(rs.have_value()) {
			rv.ivalue = atol(rs.c_str());
			rv.dvalue = atof(rs.c_str());
			rv.valid = true;
		}
	} else {			// even - get the mean of the middle two values
		snprintf(nstring, n_max_size, "%ld", ( nrecs / 2 ) - 1 );
		db->prepare_query("SELECT value FROM " + _lpo_name + " WHERE type = ? AND stamp LIKE ? ORDER BY value LIMIT 2 OFFSET ?", {type, date_like, nstring});
		while (const BWString rs = db->get_prepared_value()) {
			if(rs.have_value()) {
				rv.ivalue += atol(rs.c_str());
				rv.dvalue += atof(rs.c_str());
				rv.valid = true;
			}
		}
		if(rv.valid) {
			rv.ivalue /= 2;
			rv.dvalue /= 2;
		}
	}
	return rv;
}

BWString BWLPO::json( const BWString & date ) {
	lporesult air_mean = get_mean(_kairtemp, date);
	lporesult air_median = get_median(_kairtemp, date);
	lporesult bar_mean = get_mean(_kbarpressure, date);
	lporesult bar_median = get_median(_kbarpressure, date);
	lporesult wind_mean = get_mean(_kwindspeed, date);
	lporesult wind_median = get_median(_kwindspeed, date);

	BWString outstring;
	outstring.format(_json_format,
		 date.c_str(),
		 air_mean.dvalue,
		 air_median.dvalue,
		 bar_mean.dvalue,
		 bar_median.dvalue,
		 wind_mean.dvalue,
		 wind_median.dvalue
		 );
	return outstring;
}

BWString BWLPO::json_pair( const BWString & lhs, const BWString & rhs ) {
	BWString outstring;
	return outstring.format(_json_pair_format, lhs.c_str(), rhs.c_str());
}

