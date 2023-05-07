/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 7.5.23
 */

#pragma once


#include <string>
#include <filesystem>

class Timestamp
{
private:
	int year{}, month{}, day{}, hour{}, minute{}, second{};

public:
	Timestamp() = default;

	explicit Timestamp(const std::string& str);

	explicit Timestamp(const std::filesystem::file_time_type& filetime);

	void verify() const;

	std::string str() const;

	friend bool operator<(const Timestamp& lhs, const Timestamp& rhs);

	friend bool operator==(const Timestamp& lhs, const Timestamp& rhs);

	friend bool operator>(const Timestamp& lhs, const Timestamp& rhs);

};
