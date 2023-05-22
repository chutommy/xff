/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 14.5.23
 */

#include "Index.h"
#include "File.h"
#include "CPPFile.h"
#include "TXTFile.h"
#include "CSVFile.h"
#include "Font.h"
#include "FileInaccessible.h"

#include <fstream>
#include <set>
#include <iostream>

Index::Index(const std::filesystem::path& new_dir_path, Logger new_logger)
		: dir_path(new_dir_path),
		  index_path(new_dir_path / INDEX_NAME),
		  temp_index_path(index_path.string() + NEW_TEMP_EXT),
		  logger(new_logger)
{
	if (exists(temp_index_path) && std::remove(temp_index_path.c_str()) != 0)
		throw FileInaccessible("Inaccessible file: ", temp_index_path);
	if (!exists(index_path))
	{
		std::ofstream f(index_path.string());
		if (!f.is_open())
			throw FileInaccessible("Open file: ", index_path);
	}
}

void index_filepath(std::ofstream& os, const std::filesystem::path& path)
{
	const std::filesystem::path& ext = path.extension();
	std::shared_ptr<File> file;
	if (ext == ".txt") file = std::make_shared<TXTFile>(path);
	else if (ext == ".csv") file = std::make_shared<CSVFile>(path);
	else if (ext == ".cpp") file = std::make_shared<CPPFile>(path);
	else file = std::make_shared<File>(File(path));
	file->store(os);
}

std::shared_ptr<File> load_file(std::istringstream& iss)
{
	std::shared_ptr<File> file;
	File f(iss);
	const std::filesystem::path& ext = f.extension();
	if (ext == ".cpp") file = std::make_shared<CPPFile>(f, iss);
	else if (ext == ".txt") file = std::make_shared<TXTFile>(f, iss);
	else if (ext == ".csv") file = std::make_shared<CSVFile>(f, iss);
	else file = std::make_shared<File>(f);
	return file;
}

void Index::update() const
{
	std::ifstream orig_xff(index_path);
	if (!orig_xff.is_open())
		throw FileInaccessible("Open file: ", index_path);

	std::ofstream new_xff(temp_index_path);
	if (!new_xff.is_open())
		throw FileInaccessible("Open file: ", temp_index_path);

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
			logger.log(remove_file, filepath.relative_path());
			continue;
		}
		if (!is_regular_file(filepath))
		{
			logger.log(no_support_file, filepath.relative_path());
			continue;
		}

		if (file->up_to_date())
		{
			file->store(new_xff);
			indexed.insert(absolute(filepath));
		}
		else
		{
			logger.log(reindex_file, filepath.relative_path());
			index_filepath(new_xff, filepath);
			indexed.insert(absolute(filepath));
		}
	}

	complement_index(new_xff, indexed);

	orig_xff.close();
	if (std::remove(index_path.c_str()) != 0)
		throw FileInaccessible("Remove failure: ", index_path);

	new_xff.close();
	if (std::rename(temp_index_path.c_str(), index_path.c_str()) != 0)
		throw FileInaccessible("Rename failure: ",
				temp_index_path.string() + " -> " + index_path.string());
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
			logger.log(index_file, filepath.relative_path());
			index_filepath(new_xff, filepath);
		}
	}
}

void Index::search(std::shared_ptr<MainQuery>& query) const
{
	std::ifstream index(index_path);
	if (!index.is_open())
		throw FileInaccessible("Open file: ", index_path);

	std::stringstream buffer;
	buffer << index.rdbuf();
	std::string file_contents = buffer.str();
	std::istringstream iss(file_contents);

	int idx = 1;
	int count = 0;

	while (iss.rdbuf()->in_avail())
	{
		count++;
		std::shared_ptr<File> file = load_file(iss);
		if (query->evaluate(file))
		{
			if (idx == 1)
			{
				std::string result_title = "\n=== Search results ===";
				logger << BOLD << result_title << RESET << "\n\n";
			}
			logger.print_file(file, idx++);
		}
	}

	logger << BOLD << "\n=== Search complete. Matched files: ["
		   << idx - 1 << "/" << count << "] ===\n\n" << RESET;
}

void Index::reset() const
{
	if (std::remove(index_path.c_str()) != 0)
		throw FileInaccessible("Remove failure: ", index_path);

	std::ofstream f(index_path.string());
	if (!f.is_open())
		throw FileInaccessible("Open file: ", index_path);
	f.close();

	update();
}
