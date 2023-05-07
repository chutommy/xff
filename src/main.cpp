/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 3.5.23
 */


#include "file/File.h"
#include "file/TXTFile.h"
#include "file/CSVFile.h"
#include "file/CPPFile.h"
#include "query/Query.h"

#include <filesystem>
#include <iostream>
#include <fstream>

int main(int argc, char** argv)
{
//	std::ofstream xff_file(".xff");
//	for (const std::filesystem::directory_entry& entry:
//			std::filesystem::recursive_directory_iterator("."))
//	{
//		if (entry.is_regular_file())
//		{
//			const std::filesystem::path& path = entry.path();
//			const std::filesystem::path& ext = path.extension();
//			std::shared_ptr<File> file;
//			if (ext == ".txt")
//				file = std::make_shared<TXTFile>(path);
//			else if (ext == ".csv")
//				file = std::make_shared<CSVFile>(CSVFile(path));
//			else if (ext == ".cpp")
//				file = std::make_shared<CPPFile>(path);
//			else
//				file = std::make_shared<File>(File(path));
//			file->store(xff_file);
//		}
//	}
//	xff_file.close();
	auto q = parse(argv[1], argv[2]);

	std::ifstream xff_ifile(".xff");
	if (!xff_ifile.is_open())
	{
		std::cout << "Failed to open xff_ifile" << std::endl;
		return 1;
	}
	std::stringstream buffer;
	buffer << xff_ifile.rdbuf();
	std::string file_contents = buffer.str();
	std::istringstream iss(file_contents);
	int counter = 1;
	while (iss.rdbuf()->in_avail())
	{
		File f(iss);
		std::shared_ptr<File> file;
		const std::filesystem::path& ext = f.extension();
		if (ext == ".cpp")
			file = std::make_shared<CPPFile>(f, iss);
		else if (ext == ".txt")
			file = std::make_shared<TXTFile>(f, iss);
		else if (ext == ".csv")
			file = std::make_shared<CSVFile>(f, iss);
		else
			file = std::make_shared<File>(f);

		if (q->evaluate(file))
			file->print(std::cout << counter++ << ") ") << std::endl;
	}
}
