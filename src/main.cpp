/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 3.5.23
 */


#include "File.h"
#include "TXTFile.h"
#include "CSVFile.h"
#include "CPPFile.h"

#include <filesystem>
#include <iostream>
#include <vector>

const int TOP_WORDS_SIZE = 5;
const int LABEL_COLUMN_WIDTH = 14;

int main()
{
	std::set<std::unique_ptr<File>> files;

	for (const std::filesystem::directory_entry& entry:
			std::filesystem::recursive_directory_iterator("."))
	{
		if (entry.is_regular_file())
		{
			const std::filesystem::path& path = entry.path();
			const std::filesystem::path& ext = path.extension();
			if (ext == ".txt")
				files.insert(std::make_unique<TXTFile>(
						path,
						last_write_time(path),
						file_size(path),
						get_word_count(path),
						get_readability_score(path),
						get_most_frequent_words(path, TOP_WORDS_SIZE)));
			else if (ext == ".csv")
				files.insert(std::make_unique<CSVFile>(
						path,
						last_write_time(path),
						file_size(path),
						get_row_count(path)));
			else if (ext == ".cpp")
				files.insert(std::make_unique<CPPFile>(
						path,
						last_write_time(path),
						file_size(path),
						get_keyword_count(path),
						get_includes(path)));
			else
				files.insert(std::make_unique<File>(path, last_write_time(path),
						file_size(path)));
		}
	}

	for (const auto& file: files)
	{
		file->print(std::cout, LABEL_COLUMN_WIDTH) << std::endl;
	}

	std::cout << "Number of matching files: " << files.size() << std::endl;
}
