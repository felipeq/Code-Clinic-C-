
TARGET=nQueens
CC=c++
CPPFLAGS=-O3 -std=gnu++11

all: $(TARGET)

BWCLS.o: BWCLS.cpp BWCLS.h
	$(CC) $(CPPFLAGS) -c -o BWCLS.o BWCLS.cpp

BWString.o: BWString.cpp BWString.h
	$(CC) $(CPPFLAGS) -c -o BWString.o BWString.cpp

NQ.o: NQ.cpp NQ.h
	$(CC) $(CPPFLAGS) -c -o NQ.o NQ.cpp

${TARGET}.o: main.cpp
	$(CC) $(CPPFLAGS) -c -o ${TARGET}.o main.cpp

$(TARGET): NQ.o BWCLS.o BWString.o ${TARGET}.o
	$(CC) -o ${TARGET} ${TARGET}.o NQ.o BWCLS.o BWString.o ${LDFLAGS}

clean:
	rm $(TARGET) *.o

