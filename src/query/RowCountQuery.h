/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 7.5.23
 */

#pragma once

#include "Query.h"

class RowCountQuery : public Query
{
private:
	IntTerm term;

public:
	explicit RowCountQuery(const IntTerm& term) : term(term)
	{
	}

	bool evaluate(std::shared_ptr<File> file) override
	{
		return file->match_row_count(term);
	}
};
