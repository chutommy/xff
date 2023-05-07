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
			const Timestamp& new_last_write_time,
			size_t new_size,
			size_t new_row_count);

	std::ostream& print(std::ostream& os) const override;

	std::ostream& store(std::ostream& os) const override;

	bool MatchRowCount(const IntTerm& term) const override;
};

size_t get_row_count(const std::filesystem::path& path);
