/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 3.5.23
 */

#include "File.h"
#include "DataFileCorrupted.h"

#include <utility>
#include <algorithm>
#include <regex>

File::File(std::filesystem::path new_path,
		const Timestamp& new_last_write_time,
		size_t new_size)
		: path(std::move(new_path)),
		  last_write_time(new_last_write_time),
		  size(new_size)
{
}

File::File(const std::filesystem::path& file_path)
		: File(file_path,
		Timestamp(fs_time_to_str(
				std::filesystem::last_write_time(file_path))),
		file_size(file_path))
{
}

File::File(std::istringstream& iss)
{
	std::string path_str, size_str, last_write_time_str;
	if (!std::getline(iss, path_str)
		|| !std::getline(iss, size_str)
		|| !std::getline(iss, last_write_time_str))
		throw DataFileCorrupted("Invalid format");

	last_write_time = Timestamp(last_write_time_str);

	if (path_str.size() >= 2)
		path = path_str.substr(1, path_str.size() - 2);
	if (!exists(path) || !is_regular_file(path))
		throw DataFileCorrupted("Invalid filepath");

	if (!onlyDigits(size_str))
		throw DataFileCorrupted("Invalid size value");
	size = std::stoi(size_str);
}

bool onlyDigits(const std::string& size_str)
{
	return std::regex_match(size_str, std::regex("^[0-9]+(\\.[0-9]+)?$"));
}

std::string format_type(const std::string& extension)
{
	size_t size = extension.size();
	if (size < 2)
		return "[unspecified]";
	std::string type = extension.substr(1, size - 1);
	std::transform(type.begin(), type.end(), type.begin(), toupper);
	return type;
}

std::ostream& File::print(std::ostream& os, int width) const
{
	return os << absolute(path) << "\n"
			  << std::setw(width) << std::right << "File: " << path.filename() << "\n"
			  << std::setw(width) << std::right << "Type: " << format_type(path.extension()) << "\n"
			  << std::setw(width) << std::right << "Size: " << size << "\n"
			  << std::setw(width) << std::right << "Modify: " << last_write_time.str() << "\n";
}

std::ostream& File::store(std::ostream& os) const
{
	return os << absolute(path) << "\n"
			  << size << "\n"
			  << last_write_time.str() << "\n";
}

std::string fs_time_to_str(const std::filesystem::file_time_type& filetime)
{
	auto sys_time = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
			filetime - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());
	auto filetime_t = std::chrono::system_clock::to_time_t(sys_time);
	std::tm tm_filetime_t = *std::localtime(&filetime_t);

	char buffer[20];
	size_t buflen = std::strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", &tm_filetime_t);
	std::string filetime_str(buffer, buflen);

	return filetime_str;
}
