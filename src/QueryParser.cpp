/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 7.5.23
 */

#include "QueryParser.h"
#include "NameQuery.h"
#include "RegexNameQuery.h"
#include "SizeQuery.h"
#include "LastWriteTimeQuery.h"
#include "WordCountQuery.h"
#include "MostFrequentWordsQuery.h"
#include "ReadabilityQuery.h"
#include "RowCountQuery.h"
#include "KeywordCountQuery.h"
#include "IncludeQuery.h"
#include "InvalidQuery.h"

const char* const CMD_NAME = "name";
const char* const CMD_LIKE = "like";
const char* const CMD_SIZE_LT = "size-";
const char* const CMD_SIZE = "size";
const char* const CMD_SIZE_GT = "size+";
const char* const CMD_TIME_LT = "time-";
const char* const CMD_TIME = "time";
const char* const CMD_TIME_GT = "time+";
const char* const CMD_WORDS_LT = "words-";
const char* const CMD_WORDS = "words";
const char* const CMD_WORDS_GT = "words+";
const char* const CMD_TOP = "top";
const char* const CMD_READABILITY_LT = "readability-";
const char* const CMD_READABILITY = "readability";
const char* const CMD_READABILITY_GT = "readability+";
const char* const CMD_ROWS_LT = "rows-";
const char* const CMD_ROWS = "rows";
const char* const CMD_ROWS_GT = "rows+";
const char* const CMD_KEYWORDS_LT = "keywords-";
const char* const CMD_KEYWORDS = "keywords";
const char* const CMD_KEYWORDS_GT = "keywords+";
const char* const CMD_INCLUDE = "include";

std::shared_ptr<Query> QueryParser::operator()(const std::string& cmd, const std::string& arg)
{
	try
	{
		if (cmd == CMD_NAME) return std::make_shared<NameQuery>(StringTerm{ arg });

		else if (cmd == CMD_LIKE) return std::make_shared<RegexNameQuery>(StringTerm{ arg });

		else if (cmd == CMD_SIZE_LT) return std::make_shared<SizeQuery>(IntTerm{ std::stoi(arg), lt });
		else if (cmd == CMD_SIZE) return std::make_shared<SizeQuery>(IntTerm{ std::stoi(arg), eq });
		else if (cmd == CMD_SIZE_GT) return std::make_shared<SizeQuery>(IntTerm{ std::stoi(arg), gt });

		else if (cmd == CMD_TIME_LT) return std::make_shared<LastWriteTimeQuery>(TimestampTerm{ Timestamp(arg), lt });
		else if (cmd == CMD_TIME) return std::make_shared<LastWriteTimeQuery>(TimestampTerm{ Timestamp(arg), eq });
		else if (cmd == CMD_TIME_GT) return std::make_shared<LastWriteTimeQuery>(TimestampTerm{ Timestamp(arg), gt });

		else if (cmd == CMD_WORDS_LT) return std::make_shared<WordCountQuery>(IntTerm{ std::stoi(arg), lt });
		else if (cmd == CMD_WORDS) return std::make_shared<WordCountQuery>(IntTerm{ std::stoi(arg), eq });
		else if (cmd == CMD_WORDS_GT) return std::make_shared<WordCountQuery>(IntTerm{ std::stoi(arg), gt });

		else if (cmd == CMD_TOP) return std::make_shared<MostFrequentWordsQuery>(StringTerm{ arg });

		else if (cmd == CMD_READABILITY_LT) return std::make_shared<ReadabilityQuery>(DoubleTerm{ std::stod(arg), lt });
		else if (cmd == CMD_READABILITY) return std::make_shared<ReadabilityQuery>(DoubleTerm{ std::stod(arg), eq });
		else if (cmd == CMD_READABILITY_GT) return std::make_shared<ReadabilityQuery>(DoubleTerm{ std::stod(arg), gt });

		else if (cmd == CMD_ROWS_LT) return std::make_shared<RowCountQuery>(IntTerm{ std::stoi(arg), lt });
		else if (cmd == CMD_ROWS) return std::make_shared<RowCountQuery>(IntTerm{ std::stoi(arg), eq });
		else if (cmd == CMD_ROWS_GT) return std::make_shared<RowCountQuery>(IntTerm{ std::stoi(arg), gt });

		else if (cmd == CMD_KEYWORDS_LT) return std::make_shared<KeywordCountQuery>(IntTerm{ std::stoi(arg), lt });
		else if (cmd == CMD_KEYWORDS) return std::make_shared<KeywordCountQuery>(IntTerm{ std::stoi(arg), eq });
		else if (cmd == CMD_KEYWORDS_GT) return std::make_shared<KeywordCountQuery>(IntTerm{ std::stoi(arg), gt });

		else if (cmd == CMD_INCLUDE) return std::make_shared<IncludeQuery>(StringTerm{ arg });

		else throw InvalidQuery("Invalid command: ", cmd);
	}
	catch (InvalidQuery& e)
	{ throw e; }
	catch (std::exception& e)
	{ throw InvalidQuery("Invalid argument: ", cmd + " [" + arg + "]"); }
}
