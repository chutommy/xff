/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 7.5.23
 */

#pragma once

#include "File.h"

struct Query
{
	/**
	 * Evaluates whether the file satisfy query.
	 * @param file file to evaluate
	 * @return true if match, false otherwise
	 */
	virtual bool evaluate(std::shared_ptr<File> file) = 0;
};

/**
 * Handles and parses the command into a query.
 * @param cmd command name
 * @param arg query argument
 * @return Query
 */
std::shared_ptr<Query> parse(const std::string& cmd, const std::string& arg);
