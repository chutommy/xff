/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 3.5.23
 */

#include "Index.h"

#include <iostream>

int main(int argc, char** argv)
{
	Logger logger(std::cout);
	Index index(".", logger);
	index.update();
	std::shared_ptr<MainQuery> query = parse_query(argc, argv);
	index.search(query);
}
