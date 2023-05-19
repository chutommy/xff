/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 7.5.23
 */

#pragma once

#include "file/File.h"

struct Query
{
	/**
	 * Evaluates whether the file satisfy query.
	 * @param file file to evaluate
	 * @return true if match, false otherwise
	 */
	virtual bool evaluate(std::shared_ptr<File> file) = 0;
};
