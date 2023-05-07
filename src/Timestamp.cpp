/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 7.5.23
 */


#include "Timestamp.h"

#include <stdexcept>

Timestamp::Timestamp(int year, int month, int day,
		int hour, int minute, int second) : year(year), month(month),
											day(day), hour(hour),
											minute(minute), second(second)
{
	if (year < 1900 || year > 2100
		|| month < 0 || month > 12
		|| day < 0 || day > 31
		|| hour < 0 || hour > 23
		|| minute < 0 || minute > 59
		|| second < 0 || second > 59)
		throw std::logic_error("Invalid datetime");
}

Timestamp::Timestamp(const std::string& str)
{
	sscanf(str.c_str(), "%d-%d-%d %d:%d:%d",
			&year, &month, &day, &hour, &minute, &second);
}

std::string Timestamp::str() const
{
	char buffer[20];
	snprintf(buffer, sizeof(buffer),
			"%d-%d-%d %d:%d:%d",
			year, month, day, hour, minute, second);
	return buffer;
}
