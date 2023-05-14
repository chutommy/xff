/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 7.5.23
 */

#include "MainQuery.h"
#include "ANDQuery.h"
#include "ORQuery.h"
#include "InvalidQuery.h"

#include <cstring>

void MainQuery::add(const std::shared_ptr<Query>& q)
{
	queries.push_back(q);
}

std::shared_ptr<MainQuery> parse_query(int argc, char* const* argv)
{
	if (argc % 3 != 0)
		throw InvalidQuery("Invalid number of arguments");

	std::shared_ptr<MainQuery> query;
	bool conjunction = true, disjunction = true;
	for (int i = 3; i < argc; i += 3)
	{
		if (conjunction && strcmp(argv[i], "and") != 0)
			conjunction = false;
		if (disjunction && strcmp(argv[i], "or") != 0)
			disjunction = false;
	}
	if (conjunction)
		query = std::make_shared<ANDQuery>();
	else if (disjunction)
		query = std::make_shared<ORQuery>();
	else
		throw InvalidQuery("Invalid logical chaining");

	for (int i = 1; i < argc; i++)
	{
		size_t remaining = argc - i;
		if (remaining == 0) break;
		if (remaining == 1)
			throw InvalidQuery("Invalid number of arguments");
		query->add(parse(argv[i], argv[i + 1]));
		i += 2;
	}

	return query;
}
