/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 3.5.23
 */

#include "Root.h"

#include <iostream>

// TODO: add examples (replace playground)
//       create makefile
//       make doc
//       file and query builders

int main(int argc, char** argv)
{
	Root(".", std::cout).run(argc, argv);
}
