cmake_minimum_required(VERSION 3.25)
project(pa2_semestral)

set(CMAKE_CXX_STANDARD 17)

include_directories(src src/file src/query src/term src/error)

add_executable(pa2_semestral
        src/main.cpp src/file/File.cpp src/file/File.h src/file/TXTFile.cpp src/file/TXTFile.h src/file/CSVFile.cpp src/file/CSVFile.h src/file/CPPFile.cpp src/file/CPPFile.h src/error/DataFileCorrupted.h src/Timestamp.cpp src/Timestamp.h src/term/IntTerm.h src/term/StringTerm.h src/term/DoubleTerm.h src/term/TimestampTerm.h src/term/CompareOpt.h src/query/Query.h src/query/NameQuery.h src/query/RegexNameQuery.h src/query/SizeQuery.h src/query/LastWriteTimeQuery.h src/query/WordCountQuery.h src/query/MostFrequentWordsQuery.h src/query/ReadabilityQuery.h src/query/RowCountQuery.h src/query/KeywordCountQuery.h src/query/IncludeQuery.h src/query/ANDQuery.cpp src/query/ANDQuery.h src/query/ORQuery.cpp src/query/ORQuery.h src/query/Query.cpp src/query/MainQuery.h src/query/MainQuery.cpp src/Index.cpp src/Index.h src/Logger.cpp src/Logger.h src/error/InvalidQuery.h src/error/InvalidTimestamp.h src/Font.h src/Root.cpp src/Root.h)

target_link_libraries(pa2_semestral stdc++fs)
