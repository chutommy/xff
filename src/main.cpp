/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 3.5.23
 */

#include "Root.h"

#include <iostream>

// TODO: add examples (replace playground)
//       create makefile (https://courses.fit.cvut.cz/BI-PA2/semestral.html#_soubor-makefile)

int main(int argc, char** argv)
{
	Root(".", std::cout).run(argc, argv);
}
