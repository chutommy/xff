/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 14.5.23
 */

#pragma once

#include <stdexcept>

struct InvalidQuery : public std::invalid_argument
{
	explicit InvalidQuery(const std::string& message, const std::string& optional = "")
			: std::invalid_argument(message + optional)
	{
	}
};

