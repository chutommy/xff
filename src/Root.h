/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 15.5.23
 */

#pragma once

#include <filesystem>
#include "Logger.h"
#include "Index.h"

/**
 * Handles command-line actions.
 */
class Root
{
private:
	std::filesystem::path root_dir;
	Logger logger;
	Index index;

	bool update() const;

	bool reset() const;

	bool query(int argc, char** argv) const;

public:
	Root(const std::string& new_root_dir, std::ostream& ostream);

	/**
	 * Executes command-line arguments.
	 * @param argc argument count from console
	 * @param argv argument vector from console
	 * @return command-line return code
	 */
	int run(int argc, char** argv) const;
};
