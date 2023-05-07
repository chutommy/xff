/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 7.5.23
 */

#pragma once


#include <string>

class Timestamp
{
private:
	int year{}, month{}, day{}, hour{}, minute{}, second{};

public:
	Timestamp() = default;

	Timestamp(int year, int month, int day, int hour, int minute, int second);

	explicit Timestamp(const std::string& str);

	std::string str() const;
};
