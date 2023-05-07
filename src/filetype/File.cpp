/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 3.5.23
 */

#include "File.h"
#include "../DataFileCorrupted.h"

#include <utility>
#include <algorithm>
#include <regex>

File::File(std::filesystem::path new_path,
		const Timestamp& new_last_write_time,
		size_t new_size)
		: path(std::move(new_path)),
		  last_write_time(new_last_write_time),
		  size(new_size)
{
}

File::File(const std::filesystem::path& file_path)
		: File(file_path,
		Timestamp(std::filesystem::last_write_time(file_path)),
		file_size(file_path))
{
}

File::File(std::istringstream& iss)
{
	std::string path_str, size_str, last_write_time_str;
	if (!std::getline(iss, path_str)
		|| !std::getline(iss, size_str)
		|| !std::getline(iss, last_write_time_str))
		throw DataFileCorrupted("Invalid format");

	last_write_time = Timestamp(last_write_time_str);

	if (path_str.size() >= 2)
		path = path_str.substr(1, path_str.size() - 2);
	if (!exists(path) || !is_regular_file(path))
		throw DataFileCorrupted("Invalid filepath");

	if (!onlyDigits(size_str))
		throw DataFileCorrupted("Invalid size value");
	size = std::stoi(size_str);
}

bool onlyDigits(const std::string& size_str)
{
	return std::regex_match(size_str, std::regex("^[0-9]+(\\.[0-9]+)?$"));
}

std::string format_type(const std::string& extension)
{
	size_t size = extension.size();
	if (size < 2)
		return "[unspecified]";
	std::string type = extension.substr(1, size - 1);
	std::transform(type.begin(), type.end(), type.begin(), toupper);
	return type;
}

std::ostream& File::print(std::ostream& os) const
{
	return os << absolute(path) << "\n"
			  << std::setw(LABEL_WIDTH) << std::right << "File: " << path.filename() << "\n"
			  << std::setw(LABEL_WIDTH) << std::right << "Type: " << format_type(path.extension()) << "\n"
			  << std::setw(LABEL_WIDTH) << std::right << "Size: " << size << "\n"
			  << std::setw(LABEL_WIDTH) << std::right << "Modify: " << last_write_time.str() << "\n";
}

std::ostream& File::store(std::ostream& os) const
{
	return os << absolute(path) << "\n"
			  << size << "\n"
			  << last_write_time.str() << "\n";
}

bool File::MatchName(const StringTerm& term) const
{
	return path.stem().string() == term.value
		   || path.filename().string() == term.value;
}

bool File::MatchNameRegex(const StringTerm& term) const
{
	const std::basic_regex<char>& query = std::regex(term.value);
	return std::regex_match(path.stem().string(), query)
		   || std::regex_match(path.filename().string(), query);
}

bool File::MatchSize(const IntTerm& term) const
{
	switch (term.opt)
	{
	case lt:
		return size < term.value;
	case lte:
		return size <= term.value;
	case eq:
		return size == term.value;
	case gte:
		return size >= term.value;
	case gt:
		return size > term.value;
	default:
		throw std::runtime_error("Unexpected integer term option value");
	}
}

bool File::MatchLastWriteTime(const TimestampTerm& term) const
{
	switch (term.opt)
	{
	case lt:
		return last_write_time < term.value;
	case lte:
		return last_write_time <= term.value;
	case eq:
		return last_write_time == term.value;
	case gte:
		return last_write_time >= term.value;
	case gt:
		return last_write_time > term.value;
	default:
		throw std::runtime_error("Unexpected timestamp term option value");
	}
}

bool File::MatchWordsCount(const IntTerm& term) const
{
	return false;
}

bool File::MatchFrequentWord(const StringTerm& term) const
{
	return false;
}

bool File::MatchReadability(const DoubleTerm& term) const
{
	return false;
}

bool File::MatchRowCount(const IntTerm& term) const
{
	return false;
}

bool File::MatchKeywordCount(const IntTerm& term) const
{
	return false;
}

bool File::MatchInclude(const StringTerm& term) const
{
	return false;
}
