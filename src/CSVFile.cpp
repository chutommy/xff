/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 3.5.23
 */

#include <algorithm>
#include <fstream>
#include "CSVFile.h"

CSVFile::CSVFile(std::filesystem::path new_path,
		std::string new_last_write_time,
		size_t new_size,
		size_t new_row_count)
		: File(std::move(new_path), std::move(new_last_write_time), new_size),
		  row_count(new_row_count)
{
}

std::ostream& CSVFile::print(std::ostream& os, int width) const
{
	return File::print(os, width)
			<< std::setw(width) << std::right << "Rows: " << row_count << "\n";
}

std::ostream& CSVFile::store(std::ostream& os) const
{
	return File::store(os) << row_count << "\n";
}

size_t get_row_count(const std::filesystem::path& path)
{
	std::ifstream file(path);
	return std::count(std::istreambuf_iterator<char>(file),
			std::istreambuf_iterator<char>(), '\n');
}
