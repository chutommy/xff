/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 3.5.23
 */

#include "TXTFile.h"
#include "DataFileCorrupted.h"

#include <utility>
#include <fstream>
#include <algorithm>

TXTFile::TXTFile(std::filesystem::path new_path,
		const Timestamp& new_last_write_time,
		size_t new_size,
		size_t new_word_count,
		double new_readability,
		std::vector<std::pair<std::string, int>> new_most_frequent_words)
		: File(std::move(new_path), new_last_write_time, new_size),
		  word_count(new_word_count),
		  readability(new_readability),
		  most_frequent_words(std::move(new_most_frequent_words))
{
}

TXTFile::TXTFile(const std::filesystem::path& file_path)
		: TXTFile(file_path,
		Timestamp(std::filesystem::last_write_time(file_path)),
		file_size(file_path),
		get_word_count(file_path),
		get_readability_score(file_path),
		get_most_frequent_words(file_path, TOP_WORDS_SIZE))
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

TXTFile::TXTFile(File& file, std::istringstream& iss) : File(file)
{
	std::string word_count_str, readability_str, most_frequent_str;
	if (!std::getline(iss, word_count_str)
		|| !std::getline(iss, readability_str)
		|| !std::getline(iss, most_frequent_str))
		throw DataFileCorrupted("Invalid format");

	if (!onlyDigits(word_count_str))
		throw DataFileCorrupted("Invalid word count");
	word_count = std::stoi(word_count_str);

	if (!onlyDigits(readability_str))
		throw DataFileCorrupted("Invalid readability score");
	readability = std::stod(readability_str);

	std::stringstream mf_ss(most_frequent_str);
	while (mf_ss.rdbuf()->in_avail())
	{
		std::string word, count_str;
		mf_ss >> word;
		mf_ss >> count_str;
		if (word.empty())
			break;

		if (!onlyDigits(readability_str))
			throw DataFileCorrupted("Invalid frequent word count");
		int count = std::stoi(count_str);

		most_frequent_words.emplace_back(word, count);
	}
}

bool TXTFile::MatchWordsCount(const IntTerm& term) const
{
	switch (term.opt)
	{
	case lt:
		return word_count < term.value;
	case lte:
		return word_count <= term.value;
	case eq:
		return word_count == term.value;
	case gte:
		return word_count >= term.value;
	case gt:
		return word_count > term.value;
	default:
		throw std::runtime_error("Unexpected integer term option value");
	}
}

bool TXTFile::MatchFrequentWord(const StringTerm& term) const
{
	for (const auto& p: most_frequent_words)
		if (p.first == term.value)
			return true;
	return false;
}

bool TXTFile::MatchReadability(const DoubleTerm& term) const
{
	switch (term.opt)
	{
	case lt:
		return readability < term.value;
	case lte:
		return readability <= term.value;
	case eq:
		return readability == term.value;
	case gte:
		return readability >= term.value;
	case gt:
		return readability > term.value;
	default:
		throw std::runtime_error("Unexpected integer term option value");
	}
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
