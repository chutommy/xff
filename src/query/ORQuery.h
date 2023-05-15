/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 7.5.23
 */

#pragma once

#include "MainQuery.h"

/**
 * Represents a query joint by ORs.
 */
struct ORQuery : public MainQuery
{
	bool evaluate(std::shared_ptr<File> file) override;
};
