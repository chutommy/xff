/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 3.5.23
 */

#pragma once

#include "File.h"

#include <map>
#include <unordered_set>
#include <unordered_map>

const int TOP_WORDS_SIZE = 5;

/**
 * Represents a readable text file.
 */
class TXTFile : public File
{
private:
	int word_count;
	double readability;
	std::vector<std::pair<std::string, int>> most_frequent_words;

public:
	explicit TXTFile(const std::filesystem::path& file_path);

	explicit TXTFile(File& file, std::istringstream& iss);

	TXTFile(std::filesystem::path new_path,
			const Timestamp& new_last_write_time,
			int new_size,
			int new_word_count,
			double new_readability,
			std::vector<std::pair<std::string, int>> new_most_frequent_words);

	std::ostream& print(std::ostream& os) const override;

	std::ostream& store(std::ostream& os) const override;

	bool match_word_count(const IntTerm& term) const override;

	bool match_frequent_word(const StringTerm& term) const override;

	bool match_readability(const DoubleTerm& term) const override;
};

/**
 * Counts the number of words in the file.
 * @param path path to the file
 * @return the number of words
 */
int get_word_count(const std::filesystem::path& path);

/**
 * Evaluates readability of the file.
 * @param path path to the file
 * @return the readability score
 */
double get_readability_score(const std::filesystem::path& path);

/**
 * Retrieves the most frequent words in the file.
 * @param path path to the file
 * @param n the number of retrived words
 * @return vector of most frequent words with its count
 */
std::vector<std::pair<std::string, int>> get_most_frequent_words(const std::filesystem::path& path, int n);
