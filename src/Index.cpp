/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 14.5.23
 */

#include "Index.h"
#include "File.h"
#include "CPPFile.h"
#include "TXTFile.h"
#include "CSVFile.h"

#include <fstream>
#include <set>
#include <iostream>

const char* INDEX_NAME = ".xff";
const char* NEW_TEMP_EXT = ".new";

Index::Index(const std::filesystem::path& new_dir_path, Logger new_logger)
		: dir_path(new_dir_path),
		  index_path(new_dir_path / INDEX_NAME),
		  temp_index_path(index_path.string() + NEW_TEMP_EXT),
		  logger(new_logger)
{
	std::remove(temp_index_path.c_str());
	if (!exists(index_path))
		std::ofstream f(index_path.string());

}

void index_filepath(std::ofstream& os, const std::filesystem::path& path)
{
	const std::filesystem::path& ext = path.extension();
	std::shared_ptr<File> file;
	if (ext == ".txt")
		file = std::make_shared<TXTFile>(path);
	else if (ext == ".csv")
		file = std::make_shared<CSVFile>(CSVFile(path));
	else if (ext == ".cpp")
		file = std::make_shared<CPPFile>(path);
	else
		file = std::make_shared<File>(File(path));
	file->store(os);
}

std::shared_ptr<File> load_file(std::istringstream& iss)
{
	std::shared_ptr<File> file;
	File f(iss);
	const std::filesystem::path& ext = f.extension();
	if (ext == ".cpp")
		file = std::make_shared<CPPFile>(f, iss);
	else if (ext == ".txt")
		file = std::make_shared<TXTFile>(f, iss);
	else if (ext == ".csv")
		file = std::make_shared<CSVFile>(f, iss);
	else
		file = std::make_shared<File>(f);
	return file;
}

bool Index::update() const
{
	std::ifstream orig_xff(index_path);
	std::ofstream new_xff(temp_index_path);
	if (!orig_xff.is_open() || !new_xff.is_open())
		return false;

	std::stringstream buffer;
	buffer << orig_xff.rdbuf();
	std::string index_str = buffer.str();
	std::istringstream iss(index_str);

	std::set<std::string> indexed;
	while (iss.rdbuf()->in_avail())
	{
		std::shared_ptr<File> file = load_file(iss);
		std::filesystem::path filepath = file->get_path();
		if (!exists(filepath))
		{
			logger.log(remove_file, filepath.filename());
			continue;
		}
		if (!is_regular_file(filepath))
		{
			logger.log(no_support_file, filepath.filename());
			continue;
		}
		if (file->up_to_date())
		{
			file->store(new_xff);
			indexed.insert(absolute(filepath));
		}
		else
		{
			logger.log(reindex_file, filepath.filename());
			index_filepath(new_xff, filepath);
			indexed.insert(absolute(filepath));
		}
	}

	complement_index(new_xff, indexed);
	std::remove(index_path.c_str());
	std::rename(temp_index_path.c_str(), index_path.c_str());

	return true;
}

void Index::complement_index(std::ofstream& new_xff, const std::set<std::string>& indexed) const
{
	for (const std::filesystem::directory_entry& entry:
			std::filesystem::recursive_directory_iterator(dir_path))
	{
		const std::filesystem::path& filepath = entry.path();
		if (is_regular_file(filepath)
			&& !indexed.count(absolute(filepath))
			&& filepath != index_path
			&& filepath != temp_index_path)
		{
			logger.log(index_file, filepath.filename());
			index_filepath(new_xff, filepath);
		}
	}
}
