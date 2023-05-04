/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 3.5.23
 */

#pragma once


#include "File.h"

#include <set>

class CPPFile : public File
{
private:
	size_t keyword_count;
	std::set<std::string> includes;

public:
	CPPFile(std::filesystem::path new_path,
			std::filesystem::file_time_type new_last_write_time,
			size_t new_size,
			size_t new_keyword_count,
			std::set<std::string> new_includes);

	std::ostream& print(std::ostream& os, int width) const override;
};

size_t get_keyword_count(const std::filesystem::path& path);

std::set<std::string> get_includes(const std::filesystem::path& path);
