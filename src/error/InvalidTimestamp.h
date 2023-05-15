/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 14.5.23
 */

#pragma once

#include <stdexcept>

/**
 * Indicates invalid timestamp value.
 */
struct InvalidTimestamp : public std::logic_error
{
	explicit InvalidTimestamp(const std::string& message, const std::string& optional = "")
			: std::logic_error(message + optional)
	{
	}
};
