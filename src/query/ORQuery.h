/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 7.5.23
 */

#pragma once


#include "Query.h"

class ORQuery : public Query
{
private:
	std::vector<std::shared_ptr<Query>> queries;

public:
	bool evaluate(std::shared_ptr<File> file) override;
};
