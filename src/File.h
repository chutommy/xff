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

class File
{
protected:
	std::filesystem::path path;
	Timestamp last_write_time;
	size_t size;

public:
	explicit File(const std::filesystem::path& file_path);

	explicit File(std::istringstream& iss);

	File(std::filesystem::path new_path,
			const Timestamp& new_last_write_time,
			size_t new_size);

	virtual std::ostream& print(std::ostream& os, int width) const;

	virtual std::ostream& store(std::ostream& os) const;

	std::string extension() const
	{
		return path.extension();
	}

	bool MatchName(const StringTerm& term) const;

	bool MatchNameRegex(const StringTerm& term) const;

	bool MatchSize(const IntTerm& term) const;

	bool MatchLastWriteTime(const TimestampTerm& term) const;

	bool MatchWordsCount(const IntTerm& term) const;

	bool MatchFrequentWord(const StringTerm& term) const;

	bool MatchReadability(const DoubleTerm& term) const;

	bool MatchRowCount(const IntTerm& term) const;

	bool MatchKeywordCount(const IntTerm& term) const;

	bool MatchInclude(const StringTerm& term) const;
};

bool onlyDigits(const std::string& size_str);
