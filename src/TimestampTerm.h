/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 7.5.23
 */

#pragma once

#include "Timestamp.h"
#include "CompareOpt.h"

/**
 * Represents a timestamp logic term.
 */
struct TimestampTerm
{
	Timestamp value;
	CompareOpt opt = eq;
};
