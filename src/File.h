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

	virtual bool MatchName(const StringTerm& term) const;

	virtual bool MatchNameRegex(const StringTerm& term) const;

	virtual bool MatchSize(const IntTerm& term) const;

	virtual bool MatchLastWriteTime(const TimestampTerm& term) const;

	virtual bool MatchWordsCount(const IntTerm& term) const;

	virtual bool MatchFrequentWord(const StringTerm& term) const;

	virtual bool MatchReadability(const DoubleTerm& term) const;

	virtual bool MatchRowCount(const IntTerm& term) const;

	virtual bool MatchKeywordCount(const IntTerm& term) const;

	virtual bool MatchInclude(const StringTerm& term) const;
};

bool onlyDigits(const std::string& size_str);
