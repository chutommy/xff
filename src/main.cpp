/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 3.5.23
 */

#include "Root.h"

#include <iostream>

// TODO: help for joining (and, or)
//       add examples
//       check files open

int main(int argc, char** argv)
{
	Root(".", std::cout).run(argc, argv);
}
