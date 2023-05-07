/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 3.5.23
 */

#include "TXTFile.h"

#include <utility>
#include <fstream>
#include <algorithm>

TXTFile::TXTFile(std::filesystem::path new_path,
		std::string new_last_write_time,
		size_t new_size,
		size_t new_word_count,
		float new_readability,
		std::vector<std::pair<std::string, int>> new_most_frequent_words)
		: File(std::move(new_path), std::move(new_last_write_time), new_size),
		  word_count(new_word_count),
		  readability(new_readability),
		  most_frequent_words(std::move(new_most_frequent_words))
{
}

std::ostream& TXTFile::print(std::ostream& os, int width) const
{
	File::print(os, width)
			<< std::setw(width) << std::right << "Words: " << word_count << "\n";
	for (const auto& p: most_frequent_words)
	{
		std::stringstream ss;
		ss << "(" << p.second << ") ";
		os << std::setw(width) << std::right << ss.str() << p.first << "\n";
	}

	return os << std::setw(width) << std::right << "Readability: " << readability << "\n";
}

std::ostream& TXTFile::store(std::ostream& os) const
{
	File::store(os) << word_count << "\n"
					<< readability << "\n";
	for (const auto& p: most_frequent_words)
		os << p.first << " " << p.second << " ";

	return os << "\n";
}

double flesh_kincaid_score(int words, int sentences, int syllables)
{
	double avg_words_per_sentence = static_cast<double>(words) / std::max(sentences, 1);
	double avg_syllables_per_word = static_cast<double>(syllables) / words;
	return 206.835 - 1.015 * avg_words_per_sentence - 84.6 * avg_syllables_per_word;
}

bool is_vowel(char c)
{
	static const std::unordered_set<char> vowels = { 'a', 'e', 'i', 'o', 'u' };
	return vowels.count(static_cast<char>(std::tolower(c)));
}

bool is_end_of_sentence(char c)
{
	static const std::unordered_set<char> eos = { '.', ',', '!', '?' };
	return eos.count(static_cast<char>(std::tolower(c)));
}

int count_syllables(const std::string& word)
{
	int group_count = 0;
	bool is_syllable = false;
	for (char c: word)
	{
		if (is_vowel(c))
		{
			if (!is_syllable)
			{
				++group_count;
				is_syllable = true;
			}
		}
		else
		{
			is_syllable = false;
		}
	}

	return group_count;
}

size_t get_word_count(const std::filesystem::path& path)
{
	std::ifstream file(path);
	std::string word;
	size_t word_count = 0;
	while (file >> word)
		++word_count;
	return word_count;
}

double get_readability_score(const std::filesystem::path& path)
{
	std::ifstream file(path);
	int words = 0, sentences = 0, syllables = 0;
	std::string word;
	while (file >> word)
	{
		++words;
		syllables += count_syllables(word);
		if (is_end_of_sentence(word.back()))
			++sentences;
	}

	return flesh_kincaid_score(words, sentences, syllables);
}

std::vector<std::pair<std::string, int>> get_most_frequent_words(const std::filesystem::path& path, int n)
{
	std::ifstream file(path);
	std::unordered_map<std::string, int> word_counts;
	std::string word;
	while (file >> word)
		++word_counts[word];

	std::vector<std::pair<std::string, int>> sorted_words(word_counts.begin(), word_counts.end());
	std::sort(sorted_words.begin(), sorted_words.end(), [](const auto& a, const auto& b)
	{ return a.second > b.second; });

	return { sorted_words.begin(), std::min(sorted_words.begin() + n, sorted_words.end()) };
}
