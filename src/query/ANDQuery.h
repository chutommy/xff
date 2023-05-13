/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 7.5.23
 */

#pragma once

#include "Query.h"
#include "MainQuery.h"

/**
 * Represents a query joint by ANDs.
 */
class ANDQuery : public MainQuery
{
public:
	bool evaluate(std::shared_ptr<File> file) override;
};
