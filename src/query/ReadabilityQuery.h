/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 7.5.23
 */

#pragma once

#include "Query.h"

class ReadabilityQuery : public Query
{
private:
	DoubleTerm term;

public:
	explicit ReadabilityQuery(const DoubleTerm& term) : term(term)
	{
	}

	bool evaluate(std::shared_ptr<File> file) override
	{
		return file->matchReadability(term);
	}
};
