# make file for BWLib
# by Bill Weinman <http://bw.org/>
# Copyright (c) 2014 The BearHeart Group LLC
#

TARGET=../BWLib.a
CC=c++
CPPFLAGS=-O3 -std=gnu++11

all: $(TARGET)

BWDB.o: BWDB.h BWDB.cpp
	$(CC) $(CPPFLAGS) -c BWDB.cpp

BWString.o: BWString.h BWString.cpp
	$(CC) $(CPPFLAGS) -c BWString.cpp

BWNumber.o: BWNumber.h BWNumber.cpp
	$(CC) $(CPPFLAGS) -c BWNumber.cpp

BWCGI.o: BWCGI.h BWCGI.cpp
	$(CC) $(CPPFLAGS) -c BWCGI.cpp

BWWebGet.o: BWWebGet.h BWWebGet.cpp
	$(CC) $(CPPFLAGS) -c BWWebGet.cpp

$(TARGET): BWDB.o BWString.o BWNumber.o BWCGI.o BWWebGet.o
	ar rcs $(TARGET) BWDB.o BWString.o BWNumber.o BWCGI.o BWWebGet.o

clean:
	rm *.o $(TARGET)

