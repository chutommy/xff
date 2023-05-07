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

std::shared_ptr<Query> parse(const std::string &cmd, const std::string &arg);
