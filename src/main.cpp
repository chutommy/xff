/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 3.5.23
 */

#include "Index.h"
#include "DataFileCorrupted.h"

#include <iostream>

int main(int argc, char** argv)
{
	Logger logger(std::cout);
	Index index(".", logger);
	try
	{ index.update(); }
	catch (DataFileCorrupted&)
	{
		std::cout << "corrupted index" << std::endl;
		return 0;
	}
	catch (std::runtime_error&)
	{
		std::cout << "unexpected error, please try again" << std::endl;
		return 0;
	}

	if (argc == 2 && argv[1] == static_cast<std::string>("help"))
		std::cout << "helperino" << std::endl;

	else if (argc == 2 && argv[1] == static_cast<std::string>("reset"))
	{
		index.reset();
	}

	else if (argc != 1)
	{
		std::shared_ptr<MainQuery> query = parse_query(argc, argv);
		index.search(query);
	}
}
