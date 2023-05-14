/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 7.5.23
 */

#pragma once

#include "Query.h"

/**
 * Represents the top layer query.
 */
class MainQuery : public Query
{
protected:
	std::vector<std::shared_ptr<Query>> queries;

public:
	/**
	 * Adds a query to the main query.
	 * @param q query to add
	 */
	virtual void add(const std::shared_ptr<Query>& q);
};

/**
 * Parses query from the command line arguments.
 * @param argc argument count
 * @param argv argument vector
 * @return complete query
 */
std::shared_ptr<MainQuery> parse_query(int argc, char* const* argv);
