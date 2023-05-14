/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 14.5.23
 */

#pragma once

#include <iosfwd>

const int LOG_WIDTH = 15;

enum LogAction
{
	index_file,
	reindex_file,
	remove_file,
	no_support_file,
};

/**
 * Logger handles console logging.
 */
class Logger
{
private:
	std::ostream& os;

public:
	explicit Logger(std::ostream& new_os);

	/**
	 * Logs the action to the os
	 * @param action action to be logged
	 * @param str additional log string
	 */
	void log(LogAction action, const std::string& str) const;
};
