/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 7.5.23
 */

#pragma once

#include "Query.h"

/**
 * Represents query for file size.
 */
class SizeQuery : public Query
{
private:
	IntTerm term;

public:
	explicit SizeQuery(const IntTerm& term) : term(term)
	{
	}

	bool evaluate(std::shared_ptr<File> file) override
	{
		return file->match_size(term);
	}
};
