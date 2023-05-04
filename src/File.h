/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 3.5.23
 */

#pragma once


#include <string>
#include <filesystem>
#include <vector>

class File
{
protected:
	std::filesystem::path path;
	std::filesystem::file_time_type last_write_time;
	size_t size;

public:
	File(std::filesystem::path new_path,
			std::filesystem::file_time_type new_last_write_time,
			size_t new_size);

	virtual std::ostream& print(std::ostream& os, int width) const;
};
