/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 3.5.23
 */


#include "filetype/File.h"
#include "filetype/TXTFile.h"
#include "filetype/CSVFile.h"
#include "filetype/CPPFile.h"
#include "query/SizeQuery.h"
#include "query/IncludeQuery.h"

#include <filesystem>
#include <iostream>
#include <fstream>

int main()
{
	std::set<std::shared_ptr<File>> files;

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

	std::shared_ptr<Query> q1 = std::make_shared<SizeQuery>(IntTerm{ 700, gt });
	std::shared_ptr<Query> q2 = std::make_shared<IncludeQuery>(StringTerm{ "vector" });

	for (const auto& f: files)
		if (q1->evaluate(f) && q2->evaluate(f))
			f->print(std::cout) << std::endl;

//	for (const std::filesystem::directory_entry& entry:
//			std::filesystem::recursive_directory_iterator("."))
//	{
//		if (entry.is_regular_file())
//		{
//			const std::filesystem::path& path = entry.path();
//			const std::filesystem::path& ext = path.extension();
//			if (ext == ".txt")
//				filetype.insert(std::make_shared<TXTFile>(path));
//			else if (ext == ".csv")
//				filetype.insert(std::make_shared<CSVFile>(CSVFile(path)));
//			else if (ext == ".cpp")
//				filetype.insert(std::make_shared<CPPFile>(path));
//			else
//				filetype.insert(std::make_shared<File>(File(path)));
//		}
//	}
//
//	for (const auto& file: filetype)
//		file->print(std::cout) << std::endl;
//	std::cout << "Number of matching filetype: " << filetype.size() << std::endl;
//
//	std::ofstream xff_file(".xff");
//	for (const auto& file: filetype)
//		file->store(xff_file);
}
