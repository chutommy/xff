/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 3.5.23
 */

#include "Index.h"
#include "DataFileCorrupted.h"
#include "InvalidQuery.h"

#include <iostream>

// TODO: replace filesystem (experimental for C++14)
//       help for joining (and, or)
//       complete comments + docs
//       create root class
//       add examples

const std::string HELP_MESSAGE = R"(
Usage: xff ( | reset | help )
   or: xff ( CMD ARG ) [ ( and | or ) ( CMD ARG ) ... ]

  A command-line indexing tool to search and filter data.

Commands:
  [none]                    Update the index for files that have been modified since the last update.
  reset                     Remove and rebuild the entire index from scratch.
  help                      Display this help message.

  name                      Find files that match the specified filename (any file extension if not specified).
  like                      Find files that match the specified regular expression.
  size(-| |+)               Find files (smaller than | with exact | greater than) specified size.
  time(-| |+)               Find files modified (before | at | after) specified time.
  rows(-| |+)               Find comma-separated files with (fewer than | exact | more than) specified rows.
  words(-| |+)              Find text files with (fewer than | exact | more than) specified words.
  readability(-| |+)        Find text files with (lower than | exact | higher than) specified readability score.
  top                       Find text files that contain the specified word in their most frequent words.
  keyword(-| |+)            Find C++ files with (fewer than | exact | more than) specified number of C++ keywords.
  include                   Find C++ files that include specified keyword.

Examples:
  xff name myfile.txt                  # files named "myfile.txt"
  xff like ".*[0-9].*"                 # files containing a digit
  xff size+ 42                         # files larger than 42 bytes
  xff time "2021-05-01 00:00:00"       # files modified at midnight on May 1, 2021
  xff words- 500                       # text files with fewer than 500 words
  xff top foo                          # text files with a word "foo" among most frequent words
  xff readability- 50                  # text files with lower than 50 readability score
  xff rows+ 10                         # comma-separated files with more than 10 rows
  xff keyword 42                       # C++ files that contain 42 keywords
  xff include Logger.h                 # C++ files that include "Logger.h" header file

Note:
  * File size are specified in bytes (B).
  * Readability score ranges from 0 to 100, with higher scores indicating easier-to-read text.
  * Time is specified in ISO 8601 format "YYYY-MM-DD hh:mm:ss" (e.g., 2023-04-29 14:56:03).
)";

int main(int argc, char** argv)
{
	Logger logger(std::cout);
	Index index(".", logger);

	try
	{ index.update(); }
	catch (DataFileCorrupted& e)
	{
		logger.log_err("corrupted index file", e);
		logger << "Recovering by complete re-indexation...\n\n";
		index.reset();
	}
	catch (std::runtime_error& e)
	{
		logger.log_err("unexpected error", e);
		return 0;
	}

	if (argc == 2 && argv[1] == static_cast<std::string>("help"))
	{
		std::cout << HELP_MESSAGE << std::endl;
		return 0;
	}
	else if (argc == 2 && argv[1] == static_cast<std::string>("reset"))
	{
		index.reset();
		return 0;
	}
	else if (argc == 1)
		return 0;

	std::shared_ptr<MainQuery> query;
	try
	{ query = parse_query(argc, argv); }
	catch (InvalidQuery& e)
	{
		logger.log_err("invalid query", e);
		logger << "Type 'xxf help' to see help.\n" << std::endl;
		return 0;
	}
	index.search(query);
}
