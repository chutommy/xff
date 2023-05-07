/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 7.5.23
 */

#include "MainQuery.h"

void MainQuery::add(const std::shared_ptr<Query>& q)
{
	queries.push_back(q);
}
