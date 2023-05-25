cmake_minimum_required(VERSION 3.22)
project(chutommy)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_executable(chutommy
        src/main.cpp src/File.cpp src/File.h src/TXTFile.cpp src/TXTFile.h src/CSVFile.cpp src/CSVFile.h src/CPPFile.cpp src/CPPFile.h src/DataFileCorrupted.h src/Timestamp.cpp src/Timestamp.h src/IntTerm.h src/StringTerm.h src/DoubleTerm.h src/TimestampTerm.h src/CompareOpt.h src/Query.h src/NameQuery.h src/RegexNameQuery.h src/SizeQuery.h src/LastWriteTimeQuery.h src/WordCountQuery.h src/MostFrequentWordsQuery.h src/ReadabilityQuery.h src/RowCountQuery.h src/KeywordCountQuery.h src/IncludeQuery.h src/ANDQuery.cpp src/ANDQuery.h src/ORQuery.cpp src/ORQuery.h src/MainQuery.h src/MainQuery.cpp src/Index.cpp src/Index.h src/Logger.cpp src/Logger.h src/InvalidQuery.h src/InvalidTimestamp.h src/Font.h src/Root.cpp src/Root.h src/FileInaccessible.h src/QueryParser.cpp src/QueryParser.h)

target_link_libraries(chutommy stdc++fs)
