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
	int keyword_count;
	std::set<std::string> includes;

public:
	explicit CPPFile(const std::filesystem::path& file_path);

	explicit CPPFile(File& file, std::istringstream& iss);

	CPPFile(std::filesystem::path new_path,
			const Timestamp& new_last_write_time,
			int new_size,
			int new_keyword_count,
			std::set<std::string> new_includes);

	std::ostream& print(std::ostream& os) const override;

	std::ostream& store(std::ostream& os) const override;

	bool matchKeywordCount(const IntTerm& term) const override;

	bool matchInclude(const StringTerm& term) const override;
};

/**
 * Counts the number of C++ keyword in the file.
 * @param path path to the file
 * @return the number of keywords
 */
int get_keyword_count(const std::filesystem::path& path);

/**
 * Retrives the includes in the C++ file.
 * @param path path to the file
 * @return set of the includes
 */
std::set<std::string> get_includes(const std::filesystem::path& path);
