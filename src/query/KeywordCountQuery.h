/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 7.5.23
 */

#pragma once

#include "Query.h"

class KeywordCountQuery : public Query
{
private:
	IntTerm term;

public:
	explicit KeywordCountQuery(const IntTerm& term) : term(term)
	{
	}

	bool evaluate(std::shared_ptr<File> file) override
	{
		return file->matchKeywordCount(term);
	}
};
