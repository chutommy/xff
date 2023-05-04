/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 3.5.23
 */

#include "File.h"

#include <utility>
#include <algorithm>

File::File(std::filesystem::path new_path,
		std::filesystem::file_time_type new_last_write_time,
		size_t new_size)
		: path(std::move(new_path)),
		  last_write_time(new_last_write_time),
		  size(new_size)
{
}

std::string format_type(const std::string& extension)
{
	size_t size = extension.size();
	if (size < 2)
		return "[None]";
	std::string type = extension.substr(1, size - 1);
	std::transform(type.begin(), type.end(), type.begin(), toupper);
	return type;
}

std::ostream& File::print(std::ostream& os, int width) const
{
	auto sysTime = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
			last_write_time - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());
	auto time_t_lastWriteTime = std::chrono::system_clock::to_time_t(sysTime);
	std::tm tm_lastWriteTime = *std::localtime(&time_t_lastWriteTime);
	char buffer[80];
	std::strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", &tm_lastWriteTime);

	return os << absolute(path) << "\n"
			  << std::setw(width) << std::right << "File: " << path.filename() << "\n"
			  << std::setw(width) << std::right << "Type: " << format_type(path.extension()) << "\n"
			  << std::setw(width) << std::right << "Size: " << size << "\n"
			  << std::setw(width) << std::right << "Modify: " << buffer << "\n";
}
