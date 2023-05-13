/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 14.5.23
 */

#include <fstream>
#include "Index.h"

Index::Index(std::filesystem::path new_path) : path(std::move(new_path))
{
	if (!exists(path))
		std::ofstream f(path.string());
}

void Index::update() const
{
	// TODO: main.cpp
}
