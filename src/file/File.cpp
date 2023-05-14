/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 3.5.23
 */

#include "File.h"
#include "DataFileCorrupted.h"
#include "InvalidTimestamp.h"
#include "Font.h"

#include <utility>
#include <algorithm>
#include <regex>

File::File() : size(0)
{
}

File::File(std::filesystem::path new_path,
		const Timestamp& new_last_write_time,
		int new_size)
		: path(std::move(new_path)),
		  last_write_time(new_last_write_time),
		  size(new_size)
{
}

File::File(const std::filesystem::path& file_path)
		: File(file_path,
		Timestamp(std::filesystem::last_write_time(file_path)),
		static_cast<int>(file_size(file_path)))
{
}

File::File(std::istringstream& iss)
{
	std::string path_str, size_str, last_write_time_str;
	if (!std::getline(iss, path_str)
		|| !std::getline(iss, size_str)
		|| !std::getline(iss, last_write_time_str))
		throw DataFileCorrupted("Invalid index format");

	try
	{ last_write_time = Timestamp(last_write_time_str); }
	catch (InvalidTimestamp& e)
	{ throw DataFileCorrupted("Invalid size value: ", e.what()); }

	if (path_str.size() >= 2)
		path = path_str.substr(1, path_str.size() - 2);

	if (!only_digits(size_str))
		throw DataFileCorrupted("Invalid size value (illegal digit): ", size_str);
	try
	{ size = std::stoi(size_str); }
	catch (std::exception& e)
	{ throw DataFileCorrupted("Invalid size value: ", size_str); }
}

const std::filesystem::path& File::get_path() const
{
	return path;
}

std::string File::extension() const
{
	return path.extension();
}

bool File::up_to_date() const
{
	if (!exists(path)) return false;
	try
	{
		const Timestamp lwt = Timestamp(fs_time_to_str(std::filesystem::last_write_time(path)));
		if (lwt != last_write_time) return false;
	}
	catch (InvalidTimestamp& e)
	{ throw std::runtime_error(e.what()); }
	return true;
}

bool only_digits(const std::string& str)
{
	return std::regex_match(str, std::regex("^[0-9]+(\\.[0-9]+)?$"));
}

std::string format_type(const std::string& extension)
{
	int size = static_cast<int>(extension.size());
	if (size < 2) return "[unspecified]";

	std::string type = extension.substr(1, size - 1);
	std::transform(type.begin(), type.end(), type.begin(), toupper);
	return type;
}

std::ostream& File::print(std::ostream& os) const
{
	return os << BOLD << absolute(path) << RESET << "\n"
			  << std::setw(LABEL_WIDTH) << std::right << "File: " << path.filename() << "\n"
			  << std::setw(LABEL_WIDTH) << std::right << "Type: " << format_type(path.extension()) << "\n"
			  << std::setw(LABEL_WIDTH) << std::right << "Size: " << size << "\n"
			  << std::setw(LABEL_WIDTH) << std::right << "Modify: " << last_write_time.str() << "\n";
}

std::ostream& File::store(std::ostream& os) const
{
	return os << path.relative_path() << "\n"
			  << size << "\n"
			  << last_write_time.str() << "\n";
}

bool File::match_name(const StringTerm& term) const
{
	return path.stem().string() == term.value
		   || path.filename().string() == term.value;
}

bool File::match_name_regex(const StringTerm& term) const
{
	const std::basic_regex<char>& query = std::regex(term.value);
	return std::regex_match(path.stem().string(), query)
		   || std::regex_match(path.filename().string(), query);
}

bool File::match_size(const IntTerm& term) const
{
	switch (term.opt)
	{
	case lt:
		return size < term.value;
	case eq:
		return size == term.value;
	case gt:
		return size > term.value;
	default:
		throw std::runtime_error("Unexpected integer term option value");
	}
}

bool File::match_last_write_time(const TimestampTerm& term) const
{
	switch (term.opt)
	{
	case lt:
		return last_write_time < term.value;
	case eq:
		return last_write_time == term.value;
	case gt:
		return last_write_time > term.value;
	default:
		throw std::runtime_error("Unexpected timestamp term option value");
	}
}

bool File::match_word_count(const IntTerm& term) const
{
	return false;
}

bool File::match_frequent_word(const StringTerm& term) const
{
	return false;
}

bool File::match_readability(const DoubleTerm& term) const
{
	return false;
}

bool File::match_row_count(const IntTerm& term) const
{
	return false;
}

bool File::match_keyword_count(const IntTerm& term) const
{
	return false;
}

bool File::match_include(const StringTerm& term) const
{
	return false;
}
