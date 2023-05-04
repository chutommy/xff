/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 3.5.23
 */

#pragma once


#include "File.h"

#include <map>
#include <unordered_set>
#include <unordered_map>

class TXTFile : public File
{
private:
	size_t word_count;
	float readability;
	std::vector<std::pair<std::string, int>> most_frequent_words;

public:
	TXTFile(std::filesystem::path new_path,
			std::filesystem::file_time_type new_last_write_time,
			size_t new_size,
			size_t new_word_count,
			float new_readability,
			std::vector<std::pair<std::string, int>> new_most_frequent_words);

	std::ostream& print(std::ostream& os, int width) const override;
};

size_t get_word_count(const std::filesystem::path& path);

double get_readability_score(const std::filesystem::path& path);

std::vector<std::pair<std::string, int>> get_most_frequent_words(const std::filesystem::path& path, int n);
