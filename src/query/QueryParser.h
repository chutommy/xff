/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 7.5.23
 */

#pragma once

#include "Query.h"

#include <memory>

/**
 * Handle query parsing.
 */
class QueryParser
{
public:
	/**
	 * Handles and parses the command into a query.
	 * @param cmd command name
	 * @param arg query argument
	 * @return Query
	 */
	std::shared_ptr<Query> operator()(const std::string& cmd, const std::string& arg);
};
