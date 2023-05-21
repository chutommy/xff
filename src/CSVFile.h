/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 3.5.23
 */

#pragma once

#include "File.h"

/**
 * Represents a comma-separated file.
 */
class CSVFile : public File
{
private:
	long row_count;

public:
	explicit CSVFile(const std::filesystem::path& file_path);

	explicit CSVFile(File& file, std::istringstream& iss);

	CSVFile(std::filesystem::path new_path,
			const Timestamp& new_last_write_time,
			int new_size,
			long new_row_count);

	std::ostream& print(std::ostream& os) const override;

	std::ostream& store(std::ostream& os) const override;

	bool match_row_count(const IntTerm& term) const override;
};

/**
 * Counts the number of records (rows) in the file.
 * @param path path to the file
 * @return row count
 */
long get_row_count(const std::filesystem::path& path);
