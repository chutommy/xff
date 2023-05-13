/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 7.5.23
 */

#pragma once

#include <string>
#include <filesystem>

/**
 * Represents a point of time.
 */
struct Timestamp
{
	int year, month, day, hour, minute, second;

	Timestamp();

	explicit Timestamp(const std::string& str);

	explicit Timestamp(const std::filesystem::file_time_type& filetime);

	/**
	 * Checks whether the time is valid.
	 * Throws logic error if invalid.
	 */
	void verify() const;

	/**
	 * Return the string representation of the timestamp.
	 * @return formatted string
	 */
	std::string str() const;
};

bool operator<(const Timestamp& lhs, const Timestamp& rhs);

bool operator==(const Timestamp& lhs, const Timestamp& rhs);

bool operator>(const Timestamp& lhs, const Timestamp& rhs);
