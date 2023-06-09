/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 7.5.23
 */

#pragma once

/**
 * Represents a double logic term.
 */
struct DoubleTerm
{
	double value = 0;
	CompareOpt opt = eq;
};
