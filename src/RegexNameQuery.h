/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 7.5.23
 */

#pragma once

#include "Query.h"

#include <utility>

/**
 * Represents query for regular expression filename.
 */
class RegexNameQuery : public Query
{
private:
	StringTerm term;

public:
	explicit RegexNameQuery(StringTerm term) : term(std::move(term))
	{
	}

	bool evaluate(std::shared_ptr<File> file) override
	{
		return file->match_name_regex(term);
	}
};
