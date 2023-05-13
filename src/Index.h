/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 14.5.23
 */

#pragma once


#include <filesystem>

/**
 * Represents a filesystem index.
 */
class Index
{
private:
	std::filesystem::path path;

public:
	explicit Index(std::filesystem::path new_path);

	void update() const;
};
