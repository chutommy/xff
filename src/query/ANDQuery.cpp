/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 7.5.23
 */

#include "ANDQuery.h"

bool ANDQuery::evaluate(std::shared_ptr<File> file)
{
	for (const auto& q: queries)
		if (!q->evaluate(file))
			return false;
	return true;
}
