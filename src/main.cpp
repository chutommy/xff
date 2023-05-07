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
#include <fstream>

const int LABEL_COLUMN_WIDTH = 14;

int main()
{
	std::set<std::unique_ptr<File>> files;

	std::ifstream file(".xff");
	if (!file.is_open())
	{
		std::cout << "Failed to open file" << std::endl;
		return 1;
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string file_contents = buffer.str();
	std::istringstream iss(file_contents);

	while (iss.rdbuf()->in_avail())
	{
		File f(iss);
		const std::filesystem::path& ext = f.extension();
		if (ext == ".cpp")
			CPPFile(f, iss).print(std::cout, LABEL_COLUMN_WIDTH);
		else if (ext == ".txt")
			TXTFile(f, iss).print(std::cout, LABEL_COLUMN_WIDTH);
		else if (ext == ".csv")
			CSVFile(f, iss).print(std::cout, LABEL_COLUMN_WIDTH);
		else
			f.print(std::cout, LABEL_COLUMN_WIDTH);
	}

//	for (const std::filesystem::directory_entry& entry:
//			std::filesystem::recursive_directory_iterator("."))
//	{
//		if (entry.is_regular_file())
//		{
//			const std::filesystem::path& path = entry.path();
//			const std::filesystem::path& ext = path.extension();
//			if (ext == ".txt")
//				files.insert(std::make_unique<TXTFile>(path));
//			else if (ext == ".csv")
//				files.insert(std::make_unique<CSVFile>(CSVFile(path)));
//			else if (ext == ".cpp")
//				files.insert(std::make_unique<CPPFile>(path));
//			else
//				files.insert(std::make_unique<File>(File(path)));
//		}
//	}
//
//	for (const auto& file: files)
//		file->print(std::cout, LABEL_COLUMN_WIDTH) << std::endl;
//	std::cout << "Number of matching files: " << files.size() << std::endl;
//
//	std::ofstream xff_file(".xff");
//	for (const auto& file: files)
//		file->store(xff_file);
}
