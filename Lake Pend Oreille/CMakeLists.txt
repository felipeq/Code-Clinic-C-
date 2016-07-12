cmake_minimum_required(VERSION 3.5)
project(Lake_Pend_Oreille)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(LDFLAGS "-L. -lsqlite3")
set(SOURCE_FILES
        BWLib/BWCGI.cpp
        BWLib/BWCGI.h
        BWLib/BWDB.cpp
        BWLib/BWDB.h
        BWLib/BWNumber.cpp
        BWLib/BWNumber.h
        BWLib/BWString.cpp
        BWLib/BWString.h
        BWLib/BWWebGet.cpp
        BWLib/BWWebGet.h
        BWLPO.cpp
        BWLPO.h
        main.cpp)

add_executable(Lake_Pend_Oreille ${SOURCE_FILES})
target_link_libraries(Lake_Pend_Oreille ${LDFLAGS})