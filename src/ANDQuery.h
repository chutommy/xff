/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 7.5.23
 */

#pragma once

#include "MainQuery.h"

/**
 * Represents a query joint by ANDs.
 */
struct ANDQuery : public MainQuery
{
	bool evaluate(std::shared_ptr<File> file) override;
};
