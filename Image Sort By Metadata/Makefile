# Makefile for ImageSort

TARGET=ImageSort
CC=c++
CPPFLAGS=-I/usr/local/include -O3 -std=gnu++11
LDFLAGS=-L/usr/local/lib -lexiv2

all: $(TARGET)

BWCLS.o: BWCLS.cpp BWCLS.h
	$(CC) $(CPPFLAGS) -c -o BWCLS.o BWCLS.cpp

BWString.o: BWString.cpp BWString.h
	$(CC) $(CPPFLAGS) -c -o BWString.o BWString.cpp

ImageMeta.o: ImageMeta.cpp ImageMeta.h
	$(CC) $(CPPFLAGS) -c -o ImageMeta.o ImageMeta.cpp

${TARGET}.o: main.cpp
	$(CC) $(CPPFLAGS) -c -o ${TARGET}.o main.cpp

$(TARGET): ImageMeta.o BWCLS.o BWString.o ${TARGET}.o
	$(CC) -o ${TARGET} ${TARGET}.o ImageMeta.o BWCLS.o BWString.o ${LDFLAGS}

clean:
	rm $(TARGET) *.o

