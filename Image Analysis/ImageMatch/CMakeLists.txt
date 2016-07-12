cmake_minimum_required(VERSION 3.5)
project(ImageMatch)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(LDFLAGS "-L. -ljpeg")
set(SOURCE_FILES
    BWString.cpp
    BWString.h
    JPEGData.cpp
    JPEGData.h
    main.cpp)

add_executable(ImageMatch ${SOURCE_FILES})
target_link_libraries(ImageMatch ${LDFLAGS})