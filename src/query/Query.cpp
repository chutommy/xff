/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 7.5.23
 */

#include "Query.h"
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

std::shared_ptr<Query> parse(const std::string& cmd, const std::string& arg)
{
	try
	{
		if (cmd == "name") return std::make_shared<NameQuery>(StringTerm{ arg });

		else if (cmd == "like") return std::make_shared<RegexNameQuery>(StringTerm{ arg });

		else if (cmd == "size-") return std::make_shared<SizeQuery>(IntTerm{ std::stoi(arg), lt });
		else if (cmd == "size") return std::make_shared<SizeQuery>(IntTerm{ std::stoi(arg), eq });
		else if (cmd == "size+") return std::make_shared<SizeQuery>(IntTerm{ std::stoi(arg), gt });

		else if (cmd == "time-") return std::make_shared<LastWriteTimeQuery>(TimestampTerm{ Timestamp(arg), lt });
		else if (cmd == "time") return std::make_shared<LastWriteTimeQuery>(TimestampTerm{ Timestamp(arg), eq });
		else if (cmd == "time+") return std::make_shared<LastWriteTimeQuery>(TimestampTerm{ Timestamp(arg), gt });

		else if (cmd == "words-") return std::make_shared<WordCountQuery>(IntTerm{ std::stoi(arg), lt });
		else if (cmd == "words") return std::make_shared<WordCountQuery>(IntTerm{ std::stoi(arg), eq });
		else if (cmd == "words+") return std::make_shared<WordCountQuery>(IntTerm{ std::stoi(arg), gt });

		else if (cmd == "top") return std::make_shared<MostFrequentWordsQuery>(StringTerm{ arg });

		else if (cmd == "readability-") return std::make_shared<ReadabilityQuery>(DoubleTerm{ std::stod(arg), lt });
		else if (cmd == "readability") return std::make_shared<ReadabilityQuery>(DoubleTerm{ std::stod(arg), eq });
		else if (cmd == "readability+") return std::make_shared<ReadabilityQuery>(DoubleTerm{ std::stod(arg), gt });

		else if (cmd == "rows-") return std::make_shared<RowCountQuery>(IntTerm{ std::stoi(arg), lt });
		else if (cmd == "rows") return std::make_shared<RowCountQuery>(IntTerm{ std::stoi(arg), eq });
		else if (cmd == "rows+") return std::make_shared<RowCountQuery>(IntTerm{ std::stoi(arg), gt });

		else if (cmd == "keywords-") return std::make_shared<KeywordCountQuery>(IntTerm{ std::stoi(arg), lt });
		else if (cmd == "keywords") return std::make_shared<KeywordCountQuery>(IntTerm{ std::stoi(arg), eq });
		else if (cmd == "keywords+") return std::make_shared<KeywordCountQuery>(IntTerm{ std::stoi(arg), gt });

		else if (cmd == "include") return std::make_shared<IncludeQuery>(StringTerm{ arg });

		else throw InvalidQuery("Invalid command: ", cmd);
	}
	catch (InvalidQuery& e)
	{ throw e; }
	catch (std::exception& e)
	{ throw InvalidQuery("Invalid argument: ", cmd + " [" + arg + "]"); }
}
