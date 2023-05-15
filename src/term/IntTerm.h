/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 7.5.23
 */

#pragma once

#include "CompareOpt.h"

/**
 * Represents a int logic term.
 */
struct IntTerm
{
	int value = 0;
	CompareOpt opt = eq;
};
