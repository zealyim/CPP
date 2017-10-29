#ifndef CELL_H
#define CELL_H

#include <iostream>

// for Lab VI in CPSC 2150 by Gladys Monagan

// a cell stores two values corresponding to the row and column
struct Cell {
   public:
      int i;  // row
      int j;  // column
};

// define the insertion operatorfor output  << to have the cell output
// formatted as (i,j) namely (row #, column #)
std::ostream& operator << (std::ostream& out, Cell  x); 

#endif  

