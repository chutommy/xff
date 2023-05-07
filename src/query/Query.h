/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 7.5.23
 */

#pragma once


#include "../file/File.h"

class Query
{
public:
	virtual bool evaluate(std::shared_ptr<File> file) = 0;
};
