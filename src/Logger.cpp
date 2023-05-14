/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 14.5.23
 */

#include "Logger.h"
#include "Font.h"

#include <iostream>
#include <iomanip>

Logger::Logger(std::ostream& new_os) : os(new_os)
{
}

void Logger::log(LogAction action, const std::string& str) const
{
	os << BOLD
	   << std::setw(LOG_WIDTH)
	   << std::setfill(' ')
	   << std::left;
	switch (action)
	{
	case index_file:
		os << "indexing";
		break;
	case reindex_file:
		os << "reindexing";
		break;
	case remove_file:
		os << "removing";
		break;
	case no_support_file:
		os << "unfollow";
		break;
	}
	os << RESET << str << std::endl;
}

void Logger::log_err(const std::string& context, const std::exception& e) const
{
	std::string opt = e.what();
	if (opt.empty()) opt = "[null]";
	os << "\n  " << BOLD << "[ERROR]" << RESET
	   << " (" << context << ") "
	   << opt << "\n" << std::endl;
}

void Logger::print_file(const std::shared_ptr<File>& file, int index) const
{
	file->print(os << BOLD << index << ") " << RESET) << std::endl;
}

std::ostream& operator<<(const Logger& logger, const std::string& str)
{
	return logger.os << str;
}
