/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 14.5.23
 */

#pragma once

#include "Logger.h"

#include <filesystem>
#include <set>

/**
 * Represents a filesystem index.
 */
class Index
{
private:
	std::filesystem::path dir_path;
	std::filesystem::path index_path;
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
	 * @return true if success, false otherwise
	 */
	bool update() const;
};
