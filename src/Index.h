/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 14.5.23
 */

#pragma once

#include "Logger.h"
#include "query/MainQuery.h"

#include <filesystem>
#include <set>

const char* const INDEX_NAME = ".xff";
const char* const NEW_TEMP_EXT = ".new";

/**
 * Represents a filesystem index.
 */
class Index
{
private:
	std::filesystem::path dir_path;
	std::filesystem::path index_path;
	std::filesystem::path temp_index_path;
	Logger logger;

	/**
	 * Complements index with not indexed files.
	 * @param new_xff index file
	 * @param indexed set of already indexed files
	 */
	void complement_index(std::ofstream& new_xff, const std::set<std::string>& indexed) const;

public:
	Index(const std::filesystem::path& new_dir_path, Logger new_logger);

	/**
	 * Updates indexation and log out changes.
	 * Throws runtime error if index files can't be opened.
	 */
	void update() const;

	/**
	 * Queries over the index and finds matching files.
	 * @param query query for the file
	 */
	void search(std::shared_ptr<MainQuery>& query) const;

	/**
	 * Executes full indexation.
	 */
	void reset() const;
};
