/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 3.5.23
 */

#include <algorithm>
#include <fstream>
#include "CSVFile.h"
#include "DataFileCorrupted.h"

CSVFile::CSVFile(std::filesystem::path new_path,
		const Timestamp& new_last_write_time,
		size_t new_size,
		size_t new_row_count)
		: File(std::move(new_path), new_last_write_time, new_size),
		  row_count(new_row_count)
{
}

CSVFile::CSVFile(const std::filesystem::path& file_path)
		: CSVFile(file_path,
		Timestamp(std::filesystem::last_write_time(file_path)),
		file_size(file_path),
		get_row_count(file_path))
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

CSVFile::CSVFile(File& file, std::istringstream& iss) : File(file)
{
	std::string row_count_str;
	if (!std::getline(iss, row_count_str))
		throw DataFileCorrupted("Invalid format");

	if (!onlyDigits(row_count_str))
		throw DataFileCorrupted("Invalid row count");
	row_count = std::stoi(row_count_str);
}

bool CSVFile::MatchRowCount(const IntTerm& term) const
{
	switch (term.opt)
	{
	case lt:
		return row_count < term.value;
	case lte:
		return row_count <= term.value;
	case eq:
		return row_count == term.value;
	case gte:
		return row_count >= term.value;
	case gt:
		return row_count > term.value;
	default:
		throw std::runtime_error("Unexpected integer term option value");
	}
}

size_t get_row_count(const std::filesystem::path& path)
{
	std::ifstream file(path);
	return std::count(std::istreambuf_iterator<char>(file),
			std::istreambuf_iterator<char>(), '\n');
}
