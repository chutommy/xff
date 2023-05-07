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
	explicit CSVFile(const std::filesystem::path& file_path);

	explicit CSVFile(File& file, std::istringstream& iss);

	CSVFile(std::filesystem::path new_path,
			std::string new_last_write_time,
			size_t new_size,
			size_t new_row_count);

	std::ostream& print(std::ostream& os, int width) const override;

	std::ostream& store(std::ostream& os) const override;
};

size_t get_row_count(const std::filesystem::path& path);
