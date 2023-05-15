/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 7.5.23
 */

#pragma once

#include "Query.h"

#include <utility>

/**
 * Represents query for include directive.
 */
class IncludeQuery : public Query
{
private:
	StringTerm term;

public:
	explicit IncludeQuery(StringTerm term) : term(std::move(term))
	{
	}

	bool evaluate(std::shared_ptr<File> file) override
	{
		return file->match_include(term);
	}
};
