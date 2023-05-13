/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 3.5.23
 */

#pragma once

#include <string>
#include <filesystem>
#include <vector>

#include "Timestamp.h"
#include "IntTerm.h"
#include "StringTerm.h"
#include "DoubleTerm.h"
#include "TimestampTerm.h"

const int LABEL_WIDTH = 14;

/**
 * Represents a generic file.
 */
class File
{
protected:
	std::filesystem::path path;
	Timestamp last_write_time;
	int size;

public:
	/**
	 * Creates a new File instance from scratch.
	 * @param new_path path of the file
	 * @param new_last_write_time last modification time
	 * @param new_size size of the file
	 */
	File(std::filesystem::path new_path, const Timestamp& new_last_write_time, int new_size);

	/**
	 * Loads File from the path.
	 * @param file_path path of the loading file
	 */
	explicit File(const std::filesystem::path& file_path);

	/**
	 * Load file from the index.
	 * @param iss string fragment of the index
	 */
	explicit File(std::istringstream& iss);

	std::string extension() const;

	/**
	 * Prints out the data of the file.
	 * @param os output stream
	 * @return output stream
	 */
	virtual std::ostream& print(std::ostream& os) const;

	/**
	 * Stores the indexation of the file.
	 * @param os output stream
	 * @return output stream
	 */
	virtual std::ostream& store(std::ostream& os) const;

	virtual bool matchName(const StringTerm& term) const;

	virtual bool matchNameRegex(const StringTerm& term) const;

	virtual bool matchSize(const IntTerm& term) const;

	virtual bool matchLastWriteTime(const TimestampTerm& term) const;

	virtual bool matchWordsCount(const IntTerm& term) const;

	virtual bool matchFrequentWord(const StringTerm& term) const;

	virtual bool matchReadability(const DoubleTerm& term) const;

	virtual bool matchRowCount(const IntTerm& term) const;

	virtual bool matchKeywordCount(const IntTerm& term) const;

	virtual bool matchInclude(const StringTerm& term) const;
};

/**
 * Checks whether the string is composed of digits only.
 * @param str string to verify
 * @return true if only digits, false otherwise
 */
bool onlyDigits(const std::string& str);
