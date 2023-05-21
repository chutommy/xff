/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 7.5.23
 */

#pragma once

#include "Query.h"

/**
 * Represents query for text file word count.
 */
class WordCountQuery : public Query
{
private:
	IntTerm term;

public:
	explicit WordCountQuery(const IntTerm& term) : term(term)
	{
	}

	bool evaluate(std::shared_ptr<File> file) override
	{
		return file->match_word_count(term);
	}
};
