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

static const char* CMD_NAME = "name";
static const char* CMD_LIKE = "like";
static const char* CMD_SIZE_LT = "size-";
static const char* CMD_SIZE = "size";
static const char* CMD_SIZE_GT = "size+";
static const char* CMD_TIME_LT = "time-";
static const char* CMD_TIME = "time";
static const char* CMD_TIME_GT = "time+";
static const char* CMD_WORDS_LT = "words-";
static const char* CMD_WORDS = "words";
static const char* CMD_WORDS_GT = "words+";
static const char* CMD_TOP = "top";
static const char* CMD_READABILITY_LT = "readability-";
static const char* CMD_READABILITY = "readability";
static const char* CMD_READABILITY_GT = "readability+";
static const char* CMD_ROWS_LT = "rows-";
static const char* CMD_ROWS = "rows";
static const char* CMD_ROWS_GT = "rows+";
static const char* CMD_KEYWORDS_LT = "keywords-";
static const char* CMD_KEYWORDS = "keywords";
static const char* CMD_KEYWORDS_GT = "keywords+";
static const char* CMD_INCLUDE = "include";

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
