/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 15.5.23
 */

#include "Root.h"
#include "DataFileCorrupted.h"
#include "InvalidQuery.h"
#include "MainQuery.h"

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
  xff name file.txt                  # files named "file.txt"
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

Root::Root(const std::string& new_root_dir, std::ostream& ostream)
		: root_dir(new_root_dir),
		  logger(ostream),
		  index(root_dir, logger)
{
}

int Root::run(int argc, char** argv) const
{
	if (argc == 1)
	{
		if (!update()) return 1;
		return 0;
	}
	if (argc == 2 && argv[1] == static_cast<std::string>("help"))
	{
		logger << HELP_MESSAGE << std::endl;
		return 0;
	}
	else if (argc == 2 && argv[1] == static_cast<std::string>("reset"))
	{
		index.reset();
		return 0;
	}

	std::shared_ptr<MainQuery> query;
	try
	{ query = parse_query(argc, argv); }
	catch (InvalidQuery& e)
	{
		logger.log_err("invalid query", e);
		logger << "Type 'xxf help' to see help.\n" << std::endl;
		return 1;
	}

	if (!update()) return 1;
	index.search(query);
	return 0;
}

int Root::update() const
{
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

	return 1;
}
