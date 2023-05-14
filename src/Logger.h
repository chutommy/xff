/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 14.5.23
 */

#pragma once

#include "File.h"

#include <iosfwd>

const int LOG_WIDTH = 12;

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

	/**
	 * Logs out an error message and help text.
	 * @param context context in which the error occures
	 * @param e exception
	 */
	void log_err(const std::string& context, const std::exception& e) const;

	/**
	 * Prints out a formatted file.
	 * @param file file to print out
	 * @param index index of the file
	 */
	void print_file(const std::shared_ptr<File>& file, int index) const;

	friend std::ostream& operator<<(const Logger& logger, const std::string& str);
};
