
TARGET=imgmatch
CC=c++
CPPFLAGS=-O3 -std=gnu++11
LDFLAGS=-ljpeg

all: $(TARGET)

BWString.o: BWString.cpp BWString.h
	$(CC) $(CPPFLAGS) -c -o BWString.o BWString.cpp

JPEGData.o: JPEGData.cpp JPEGData.h
	$(CC) $(CPPFLAGS) -c -o JPEGData.o JPEGData.cpp

${TARGET}.o: main.cpp
	$(CC) $(CPPFLAGS) -c -o ${TARGET}.o main.cpp

$(TARGET): BWString.o JPEGData.o ${TARGET}.o
	$(CC) -o ${TARGET} ${TARGET}.o BWString.o JPEGData.o ${LDFLAGS}

clean:
	rm $(TARGET) *.o

