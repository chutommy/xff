/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 3.5.23
 */


#include <filesystem>
#include <iostream>
#include <vector>
#include "File.h"
#include "TXTFile.h"
#include "CSVFile.h"
#include "CPPFile.h"

int main()
{
	std::vector<std::unique_ptr<File>> files;

	for (const std::filesystem::directory_entry& entry:
			std::filesystem::recursive_directory_iterator("."))
	{
		if (entry.is_regular_file())
		{
			const std::filesystem::path& path = entry.path();
			const std::filesystem::path& ext = path.extension();
			if (ext == "txt")
				files.push_back(std::make_unique<TXTFile>(ext, path.filename(),
						path.parent_path(), last_write_time(path),
						file_size(path), 666, 42.42, std::map<std::string, int>{}));
			else if (ext == "csv")
				files.push_back(std::make_unique<CSVFile>(ext, path.filename(),
						path.parent_path(), last_write_time(path),
						file_size(path), 42));
			else if (ext == "cpp")
				files.push_back(std::make_unique<CPPFile>(ext, path.filename(),
						path.parent_path(), last_write_time(path),
						file_size(path), 666, std::vector<std::string>{ "hello", "hi" }));
			else
				files.push_back(std::make_unique<File>(ext, path.filename(),
						path.parent_path(), last_write_time(path),
						file_size(path)));
		}
	}
}
