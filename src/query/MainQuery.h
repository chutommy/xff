/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 7.5.23
 */

#pragma once


#include "Query.h"

class MainQuery : public Query
{
protected:
	std::vector<std::shared_ptr<Query>> queries;

public:
	virtual void add(const std::shared_ptr<Query>& q);
};

