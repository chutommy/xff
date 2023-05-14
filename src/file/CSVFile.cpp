/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 3.5.23
 */

#include "CSVFile.h"
#include "DataFileCorrupted.h"

#include <algorithm>
#include <fstream>

CSVFile::CSVFile(std::filesystem::path new_path,
		const Timestamp& new_last_write_time,
		int new_size,
		long new_row_count)
		: File(std::move(new_path), new_last_write_time, new_size),
		  row_count(new_row_count)
{
}

CSVFile::CSVFile(const std::filesystem::path& file_path)
		: CSVFile(file_path,
		Timestamp(std::filesystem::last_write_time(file_path)),
		static_cast<int>(file_size(file_path)),
		get_row_count(file_path))
{
}

std::ostream& CSVFile::print(std::ostream& os) const
{
	return File::print(os)
			<< std::setw(LABEL_WIDTH) << std::right << "Rows: " << row_count << "\n";
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

	if (!only_digits(row_count_str))
		throw DataFileCorrupted("Invalid row count");
	row_count = std::stoi(row_count_str);
}

bool CSVFile::match_row_count(const IntTerm& term) const
{
	switch (term.opt)
	{
	case lt:
		return row_count < term.value;
	case eq:
		return row_count == term.value;
	case gt:
		return row_count > term.value;
	}
}

long get_row_count(const std::filesystem::path& path)
{
	std::ifstream file(path);
	return std::count(std::istreambuf_iterator<char>(file),
			std::istreambuf_iterator<char>(), '\n');
}
