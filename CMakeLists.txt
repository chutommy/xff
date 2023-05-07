cmake_minimum_required(VERSION 3.25)
project(pa2_semestral)

set(CMAKE_CXX_STANDARD 17)

add_executable(pa2_semestral
        src/main.cpp src/File.cpp src/File.h src/TXTFile.cpp src/TXTFile.h src/CSVFile.cpp src/CSVFile.h src/CPPFile.cpp src/CPPFile.h src/Index.cpp src/Index.h src/DataFileCorrupted.h src/Timestamp.cpp src/Timestamp.h)
