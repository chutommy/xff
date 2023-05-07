cmake_minimum_required(VERSION 3.25)
project(pa2_semestral)

set(CMAKE_CXX_STANDARD 17)

add_executable(pa2_semestral
        src/main.cpp src/filetype/File.cpp src/filetype/File.h src/filetype/TXTFile.cpp src/filetype/TXTFile.h src/filetype/CSVFile.cpp src/filetype/CSVFile.h src/filetype/CPPFile.cpp src/filetype/CPPFile.h src/Index.cpp src/Index.h src/DataFileCorrupted.h src/Timestamp.cpp src/Timestamp.h src/IntTerm.h src/StringTerm.h src/DoubleTerm.h src/TimestampTerm.h src/CompareOpt.h src/query/Query.h src/query/NameQuery.h src/query/RegexNameQuery.h src/query/SizeQuery.h src/query/LastWriteTimeQuery.h src/query/WordCountQuery.h src/query/FrequentWordsQuery.h src/query/ReadabilityQuery.h src/query/RowCountQuery.h src/query/KeywordCountQuery.h src/query/IncludeQuery.h)
