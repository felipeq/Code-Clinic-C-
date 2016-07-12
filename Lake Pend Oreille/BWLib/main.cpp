//  main.cpp
//  Test bed for BWLIb
//  by Bill Weinman <http://bw.org/>
//  Copyright (c) 2014 The BearHeart Group LLC
//

#include <cstdio>
#include <vector>
#include <memory>
#include <map>
#include "BWKV.h"
#include "BWString.h"
#include "BWNumber.h"
#include "BWWebGet.h"
#include "BWCGI.h"

// Here be dragons
const char * const database_filename = "/Users/billw/sqlite3_data/test_database.db";

int main(int argc, const char * argv[])
{
	std::unique_ptr<BWKV> db( new BWKV(database_filename) );

	// Versions n things
	printf("BWDB version: %s\n", BWDB::version());
	printf("BWKV version: %s\n", BWKV::version());
	printf("BWString version: %s\n", BWString::version());
	printf("BWNumber version: %s\n", BWNumber::version());
	printf("BWWebGet version: %s\n", BWWebGet::version());
	printf("BWCGI version: %s\n", BWCGI::version());
	printf("SQLite version: %s\n",db->get_query_value("SELECT sqlite_version()").c_str());
	printf("BWKV table version: %s\n", db->get_query_value("SELECT value FROM kv WHERE key = ?", {"db_version"}).c_str());

	// let's test BWDB & BWKV !!
	printf("\nBWDB & BWKV -----\n\n");

	printf("got table?: kv (%d), foo (%d)\n", db->table_exists("kv"), db->table_exists("foo"));

	if (!db->status()) {
		printf("database failed to open.\n");
		return 0;
	}

	printf("db->value()\n");
	const char * s = db->value("db_version");
	if (s) {
		printf("kv db_version is %s\n", s);
	} else {
		printf("error: %s\n", db->get_error());
	}

	db->value("this", "that");
	printf("this -> that (%s)\n", db->get_error());
	s = db->value("this");
	if (s) {
		printf("this is %s\n", s);
	} else {
		printf("No value (%s)\n", db->get_error());
	}

	db->value("this", "other");
	printf("this -> other (%s)\n", db->get_error());
	s = db->value("this");
	if (s) {
		printf("this is %s\n", s);
	} else {
		printf("No value (%s)\n", db->get_error());
	}

	s = db->value("foo");
	if (s) {
		printf("foo is %s\n", s);
	} else {
		printf("foo: no value (%s)\n", db->get_error());
	}

	db->value("foo", "bar");
	s = (*db)["foo"];
	if (s) {
		printf("foo is %s\n", s);
	} else {
		printf("foo: no value (%s)\n", db->get_error());
	}

	printf("db->prepare_query()\n");
	printf("SELECT * FROM kv -- \n");
	db->prepare_query("SELECT * FROM kv", {});
	while (const BWDB::dbrow * r = db->get_prepared_row()) {
		for (auto col : *r ) {
			printf("%s:%s ", col.first.c_str(), col.second.c_str());
		}
		printf("\n");
	}

	printf("SELECT value FROM kv -- \n");
	db->prepare_query("SELECT value FROM kv", {});
	while (BWString rs = db->get_prepared_value()) {
		printf("rs is [%s]\n", rs.c_str());
	}

	printf("db->count()\n");
	printf("there are %ld rows in the kv table.\n", db->count("kv"));
	printf("db->del(foo)\n");
	db->del("foo");
	printf("there are %ld rows in the kv table.\n", db->count("kv"));

	db.reset();

//	// BWString
//	printf("\nBWString -----\n\n");
//
//	const char * _ctest = "   \tfoo   \r\n";
//	BWString bwtest = _ctest;
//	bwtest.trim();
//	printf("[%s] [%s] %ld\n", _ctest, bwtest.c_str(), bwtest.length());
//
//	BWString x = "foo";
//	BWString y = "bar";
//	BWString z = x + "baz" + y;
//	printf("x [%s] y [%s] z [%s] (foobazbar)\n", x.c_str(), y.c_str(), z.c_str());
//
//	x = y;
//	printf("x is now [%s]\n", x.c_str());
//
//    printf("x %s %s == y %s\n", x.c_str(), x == y ? "is" : "is not", y.c_str());
//    printf("x %s %s > y %s\n", x.c_str(), x > y ? "is" : "is not", y.c_str());
//    printf("x %s %s >= y %s\n", x.c_str(), x >= y ? "is" : "is not", y.c_str());
//    printf("x %s %s < y %s\n", x.c_str(), x < y ? "is" : "is not", y.c_str());
//    printf("x %s %s <= y %s\n", x.c_str(), x <= y ? "is" : "is not", y.c_str());
//
//	y = "foo";
//    printf("x %s %s == y %s\n", x.c_str(), x == y ? "is" : "is not", y.c_str());
//    printf("x %s %s > y %s\n", x.c_str(), x > y ? "is" : "is not", y.c_str());
//    printf("x %s %s >= y %s\n", x.c_str(), x >= y ? "is" : "is not", y.c_str());
//    printf("x %s %s < y %s\n", x.c_str(), x < y ? "is" : "is not", y.c_str());
//    printf("x %s %s <= y %s\n", x.c_str(), x <= y ? "is" : "is not", y.c_str());
//
//	x = "foo"; y = "bar";
//    printf("x %s %s == y %s\n", x.c_str(), x == y ? "is" : "is not", y.c_str());
//    printf("x %s %s > y %s\n", x.c_str(), x > y ? "is" : "is not", y.c_str());
//    printf("x %s %s >= y %s\n", x.c_str(), x >= y ? "is" : "is not", y.c_str());
//    printf("x %s %s < y %s\n", x.c_str(), x < y ? "is" : "is not", y.c_str());
//    printf("x %s %s <= y %s\n", x.c_str(), x <= y ? "is" : "is not", y.c_str());
//
//    puts(x.format("this is %s, that is %s\n", x.c_str(), y.c_str()).c_str());
	
//	// BWNumber
//	printf("\nBWNumber -----\n\n");
//
//	BWNumber a = 1;
//	BWNumber b = 2;
//	BWNumber c = 3;
//
//	printf("a: %ld %lf\n", (long) a, (double) a);
//	printf("b: %ld %lf\n", (long) b, (double) b);
//	printf("c: %ld %lf\n", (long) c, (double) c);
//
//	a = b + c;
//	b += 2;
//	c = b + (BWNumber)3;
//
//	printf("a: %ld %lf\n", (long) a, (double) a);
//	printf("b: %ld %lf\n", (long) b, (double) b);
//	printf("c: %ld %lf\n", (long) c, (double) c);
//
//	printf("a: %ld\n", (long) a++);
//	printf("a: %ld\n", (long) ++a);
//	printf("a: %ld\n", (long) a--);
//	printf("a: %ld\n", (long) --a);
//	printf("a: %lf\n", (double) a++);
//	printf("a: %lf\n", (double) ++a);
//	printf("a: %lf\n", (double) a--);
//	printf("a: %lf\n", (double) --a);
//
//	printf("a: %ld %lf\n\n", (long) a, (double) a);
//
//	a = b;
//    printf("a (%ld/%lf) %s > b (%ld/%lf)\n", (long) a, (double) a, a > b ? "is" : "is not", (long) b, (double) b);
//    printf("a (%ld/%lf) %s < b (%ld/%lf)\n", (long) a, (double) a, a < b ? "is" : "is not", (long) b, (double) b);
//    printf("a (%ld/%lf) %s == b (%ld/%lf)\n", (long) a, (double) a, a == b ? "is" : "is not", (long) b, (double) b);
//    printf("a (%ld/%lf) %s >= b (%ld/%lf)\n", (long) a, (double) a, a >= b ? "is" : "is not", (long) b, (double) b);
//    printf("a (%ld/%lf) %s <= b (%ld/%lf)\n\n", (long) a, (double) a, a <= b ? "is" : "is not", (long) b, (double) b);
//
//	a++;
//    printf("a (%ld/%lf) %s > b (%ld/%lf)\n", (long) a, (double) a, a > b ? "is" : "is not", (long) b, (double) b);
//    printf("a (%ld/%lf) %s < b (%ld/%lf)\n", (long) a, (double) a, a < b ? "is" : "is not", (long) b, (double) b);
//    printf("a (%ld/%lf) %s == b (%ld/%lf)\n", (long) a, (double) a, a == b ? "is" : "is not", (long) b, (double) b);
//    printf("a (%ld/%lf) %s >= b (%ld/%lf)\n", (long) a, (double) a, a >= b ? "is" : "is not", (long) b, (double) b);
//    printf("a (%ld/%lf) %s <= b (%ld/%lf)\n\n", (long) a, (double) a, a <= b ? "is" : "is not", (long) b, (double) b);
//
//	b += 3.2;
//    printf("a (%ld/%lf) %s > b (%ld/%lf)\n", (long) a, (double) a, a > b ? "is" : "is not", (long) b, (double) b);
//    printf("a (%ld/%lf) %s < b (%ld/%lf)\n", (long) a, (double) a, a < b ? "is" : "is not", (long) b, (double) b);
//    printf("a (%ld/%lf) %s == b (%ld/%lf)\n", (long) a, (double) a, a == b ? "is" : "is not", (long) b, (double) b);
//    printf("a (%ld/%lf) %s >= b (%ld/%lf)\n", (long) a, (double) a, a >= b ? "is" : "is not", (long) b, (double) b);
//    printf("a (%ld/%lf) %s <= b (%ld/%lf)\n\n", (long) a, (double) a, a <= b ? "is" : "is not", (long) b, (double) b);
//
//	a.reset();
//	printf("a: %ld %lf\n", (long) a, (double) a);
//
//	// BWWebGet
//	printf("\nBWWebGet -----\n\n");
//
//	// lpo.dt.navy.mil/data/DM/2014/2014_02_27/Wind_Gust
//	
//	BWWebGet web("cpp.bw.org");
//	BWString body;
//
//	printf("host: %s, port %d, address: %s\n", web.host().c_str(), (int) web.port(), web.address());
//	if (web.errno()) {
//		printf("could not connect\n");
//	} else {
//		printf("connected...\n");
//		body = web.fetch("/code-clinic-test/test-data-2014-02-27.txt");
//		if(web.errno()) printf("%s\n", web.errstr());
//		else {
//			printf("HTTP Status code: %d\n", (int) web.http_status_code());
//			std::vector<BWString> lines = body.split("\n");
//			printf("size of vector: %ld\n", lines.size());
//			for (BWString l : lines) {
//				printf("[%s]\n", l.trim().c_str());
//			}
//		}
//	}

//	// BWCGI
//	// printf("\nBWCGI -----\n\n");

//	BWCGI cgi;
//
//	if(cgi.have_cgi()) cgi.disp_page("text/plain", "this is the body.\n");
//	if(cgi.status().have_value()) printf("%s\n", cgi.status().c_str());
//
//	printf("have_query_string: %d\n", cgi.have_query_string());
//
//	for ( auto m : cgi.env() ) {
//		printf("%s [%s]\n", m.first.c_str(), m.second.c_str());
//	}
//
//	printf("CGI: %s\n", cgi.getvar("GATEWAY_INTERFACE").c_str());
//
//	for ( auto q : cgi.qmap() ) {
//		printf("%s [%s]\n", q.first.c_str(), q.second.c_str());
//	}

	return 0;	// Done. Yay!
}

