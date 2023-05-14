/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 3.5.23
 */

#include "File.h"
#include "TXTFile.h"
#include "CSVFile.h"
#include "CPPFile.h"
#include "Query.h"
#include "ANDQuery.h"
#include "ORQuery.h"
#include "DataFileCorrupted.h"
#include "Index.h"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <cstring>

void load_and_print(std::shared_ptr<MainQuery>& query)
{
	std::ifstream xff_ifile(".xff");
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

		if (query->evaluate(file))
			file->print(std::cout << counter++ << ") ") << std::endl;
	}
}

std::shared_ptr<MainQuery> create_query(int argc, char* const* argv)
{
	if (argc % 3 != 0)
		throw std::logic_error("Invalid number of arguments");

	std::shared_ptr<MainQuery> query;
	bool conjunction = true, disjunction = true;
	for (int i = 3; i < argc; i += 3)
	{
		if (conjunction && strcmp(argv[i], "and") != 0)
			conjunction = false;
		if (disjunction && strcmp(argv[i], "or") != 0)
			disjunction = false;
	}
	if (conjunction)
		query = std::make_shared<ANDQuery>();
	else if (disjunction)
		query = std::make_shared<ORQuery>();
	else
		throw std::logic_error("Invalid logical chaining");

	for (int i = 1; i < argc; i++)
	{
		size_t remaining = argc - i;
		if (remaining == 0)
			break;
		if (remaining == 1)
			throw std::logic_error("Invalid number of arguments");
		query->add(parse(argv[i], argv[i + 1]));
		i += 2;
	}
	return query;
}

int main(int argc, char** argv)
{
	Logger logger(std::cout);
	Index index(".", logger);
	index.update();

//	std::shared_ptr<MainQuery> query = create_query(argc, argv);
//	load_and_print(query);
}
