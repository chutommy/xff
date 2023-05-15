/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 3.5.23
 */

#include "Root.h"

#include <iostream>

// TODO: replace filesystem (experimental for C++14)
//       help for joining (and, or)
//       comments + doxy
//       add examples
//       check files open
//       import clean up


int main(int argc, char** argv)
{
	Root(".", std::cout).run(argc, argv);
}
