
TARGET=lpo
CC=c++
CPPFLAGS=-O3 -std=gnu++11 -IBWLib/
LDFLAGS=-L. -lsqlite3
BWLib=BWLib.a

Lpath=BWLib

all: $(TARGET)

$(BWLib):
	@$(MAKE) -C BWLib all

$(TARGET): $(BWLib) BWLPO.cpp BWLPO.h main.cpp
	$(CC) $(CPPFLAGS) $(LDFLAGS) -o lpo BWLPO.cpp main.cpp $(BWLib)

clean:
	@$(MAKE) -C BWLib clean
	rm $(TARGET)
