/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 14.5.23
 */

#include "Logger.h"

#include <iostream>
#include <iomanip>

Logger::Logger(std::ostream& new_os) : os(new_os)
{
}

void Logger::log(LogAction action, const std::string& str) const
{
	os << std::setw(LOG_WIDTH);
	switch (action)
	{
	case index_file:
		os << "indexing | ";
		break;
	case reindex_file:
		os << "reindexing | ";
		break;
	case remove_file:
		os << "removing | ";
		break;
	case no_support_file:
		os << "unfollow | ";
		break;
	}
	os << str << std::endl;
}

void Logger::print_file(const std::shared_ptr<File>& file, int index) const
{
	file->print(os << index << ") ") << std::endl;
}

std::ostream& operator<<(const Logger& logger, const std::string& str)
{
	return logger.os << str;
}
