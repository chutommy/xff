/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 7.5.23
 */


#include "Timestamp.h"

#include <stdexcept>
#include <chrono>
#include <filesystem>

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

Timestamp::Timestamp(const std::string& str)
{
	sscanf(str.c_str(), "%d-%d-%d %d:%d:%d",
			&year, &month, &day, &hour, &minute, &second);
	verify();
}

Timestamp::Timestamp(const std::filesystem::file_time_type& filetime)
		: Timestamp(fs_time_to_str(filetime))
{
	verify();
}

std::string Timestamp::str() const
{
	char buffer[20];
	snprintf(buffer, sizeof(buffer),
			"%d-%d-%d %d:%d:%d",
			year, month, day, hour, minute, second);
	return buffer;
}

bool operator<(const Timestamp& lhs, const Timestamp& rhs)
{
	if (lhs.year != rhs.year) return lhs.year < rhs.year;
	if (lhs.month != rhs.month) return lhs.month < rhs.month;
	if (lhs.day != rhs.day) return lhs.day < rhs.day;
	if (lhs.hour != rhs.hour) return lhs.hour < rhs.hour;
	if (lhs.minute != rhs.minute) return lhs.minute < rhs.minute;
	return lhs.second < rhs.second;
}

bool operator==(const Timestamp& lhs, const Timestamp& rhs)
{
	return !(lhs < rhs)
		   && !(rhs < lhs);
}

bool operator>(const Timestamp& lhs, const Timestamp& rhs)
{
	return rhs < lhs;
}

void Timestamp::verify() const
{
	if (year < 1900 || year > 2100
		|| month < 0 || month > 12
		|| day < 0 || day > 31
		|| hour < 0 || hour > 23
		|| minute < 0 || minute > 59
		|| second < 0 || second > 59)
		throw std::logic_error("Invalid datetime");
}
