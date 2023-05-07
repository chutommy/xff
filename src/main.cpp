/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 3.5.23
 */


#include "file/File.h"
#include "file/TXTFile.h"
#include "file/CSVFile.h"
#include "file/CPPFile.h"

#include <filesystem>
#include <iostream>
#include <fstream>

int main()
{
	std::set<std::shared_ptr<File>> files;

	for (const std::filesystem::directory_entry& entry:
			std::filesystem::recursive_directory_iterator("."))
	{
		if (entry.is_regular_file())
		{
			const std::filesystem::path& path = entry.path();
			const std::filesystem::path& ext = path.extension();
			if (ext == ".txt")
				files.insert(std::make_shared<TXTFile>(path));
			else if (ext == ".csv")
				files.insert(std::make_shared<CSVFile>(CSVFile(path)));
			else if (ext == ".cpp")
				files.insert(std::make_shared<CPPFile>(path));
			else
				files.insert(std::make_shared<File>(File(path)));
		}
	}
	std::ofstream xff_file(".xff");
	for (const auto& f: files)
		f->store(xff_file);

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
			files.insert(std::make_shared<CPPFile>(f, iss));
		else if (ext == ".txt")
			files.insert(std::make_shared<TXTFile>(f, iss));
		else if (ext == ".csv")
			files.insert(std::make_shared<CSVFile>(f, iss));
		else
			files.insert(std::make_shared<File>(f));
	}

	for (const auto& f: files)
		f->print(std::cout) << std::endl;
}
