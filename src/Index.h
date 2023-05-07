/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 3.5.23
 */

#pragma once


#include "File.h"

#include <vector>
#include <set>

class Index
{
private:
	std::set<std::unique_ptr<File>> files;

public:

};