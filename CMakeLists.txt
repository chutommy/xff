cmake_minimum_required(VERSION 3.25)
project(pa2_semestral)

set(CMAKE_CXX_STANDARD 17)

add_executable(pa2_semestral
        src/main.cpp src/file/File.cpp src/file/File.h src/file/TXTFile.cpp src/file/TXTFile.h src/file/CSVFile.cpp src/file/CSVFile.h src/file/CPPFile.cpp src/file/CPPFile.h src/Index.cpp src/Index.h src/DataFileCorrupted.h src/Timestamp.cpp src/Timestamp.h src/IntTerm.h src/StringTerm.h src/DoubleTerm.h src/TimestampTerm.h src/CompareOpt.h src/query/Query.h src/query/NameQuery.h src/query/RegexNameQuery.h src/query/SizeQuery.h src/query/LastWriteTimeQuery.h src/query/WordCountQuery.h src/query/MostFrequentWordsQuery.h src/query/ReadabilityQuery.h src/query/RowCountQuery.h src/query/KeywordCountQuery.h src/query/IncludeQuery.h src/query/ANDQuery.cpp src/query/ANDQuery.h src/query/ORQuery.cpp src/query/ORQuery.h src/query/Query.cpp src/query/MainQuery.h src/query/MainQuery.cpp)
