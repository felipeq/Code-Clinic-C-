# Makefile for csvweb

TARGET=csvweb
CC=c++
CPPFLAGS=-I/usr/local/include -O3 -std=gnu++11

all: $(TARGET)

BWXML.o: BWXML.cpp BWXML.h
	$(CC) $(CPPFLAGS) -c -o BWXML.o BWXML.cpp

BWCLS.o: BWCLS.cpp BWCLS.h
	$(CC) $(CPPFLAGS) -c -o BWCLS.o BWCLS.cpp

BWString.o: BWString.cpp BWString.h
	$(CC) $(CPPFLAGS) -c -o BWString.o BWString.cpp

CSV.o: CSV.cpp CSV.h
	$(CC) $(CPPFLAGS) -c -o CSV.o CSV.cpp

${TARGET}.o: main.cpp
	$(CC) $(CPPFLAGS) -c -o ${TARGET}.o main.cpp

$(TARGET): CSV.o BWCLS.o BWXML.o BWString.o ${TARGET}.o
	$(CC) -o ${TARGET} ${TARGET}.o CSV.o BWCLS.o BWXML.o BWString.o

clean:
	rm $(TARGET) *.o

