/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 3.5.23
 */

#include "Index.h"
#include "DataFileCorrupted.h"
#include "InvalidQuery.h"

#include <iostream>

// TODO: help message, terminal colors, fs, const config, complete comments, docs, tests

int main(int argc, char** argv)
{
	// TODO: split (create root app)
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
		std::cout << "help message" << std::endl;
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
