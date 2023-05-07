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
	std::string last_write_time;
	size_t size;

public:
	explicit File(const std::filesystem::path& file_path);

	explicit File(std::istringstream& iss);

	File(std::filesystem::path new_path,
			std::string new_last_write_time,
			size_t new_size);

	virtual std::ostream& print(std::ostream& os, int width) const;

	virtual std::ostream& store(std::ostream& os) const;

	std::string extension() const
	{
		return path.extension();
	}
};

std::string fs_time_to_str(const std::filesystem::file_time_type& filetime);

bool onlyDigits(const std::string& size_str);
