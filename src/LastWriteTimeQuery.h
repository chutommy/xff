/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 7.5.23
 */

#pragma once

#include "Query.h"

/**
 * Represents query for last time write (time of last modification).
 */
class LastWriteTimeQuery : public Query
{
private:
	TimestampTerm term;

public:
	explicit LastWriteTimeQuery(const TimestampTerm& term) : term(term)
	{
	}

	bool evaluate(std::shared_ptr<File> file) override
	{
		return file->match_last_write_time(term);
	}
};
