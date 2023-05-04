/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 3.5.23
 */

#pragma once


#include "File.h"

class CSVFile : public File
{
private:
	size_t row_count;

public:
	CSVFile(std::filesystem::path new_path,
			std::filesystem::file_time_type new_last_write_time,
			size_t new_size,
			size_t new_row_count);

	std::ostream& print(std::ostream& os, int width) const override;
};

size_t get_row_count(const std::filesystem::path& path);
