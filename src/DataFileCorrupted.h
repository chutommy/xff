/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 7.5.23
 */

#pragma once

#include <stdexcept>

struct DataFileCorrupted : public std::runtime_error
{
	explicit DataFileCorrupted(const std::string& message)
			: std::runtime_error(message)
	{
	}
};
