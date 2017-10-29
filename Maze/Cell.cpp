#include "Cell.h"

// for Lab VI in CPSC 2150 by Gladys Monagan

#include <iostream>

// define the insertion operator << to have the cell formatted as (i,j)
std::ostream& operator << (std::ostream& out, Cell  x) {
   out << " (" << x.i << "," << x.j << ")" ;
   return out;
}

