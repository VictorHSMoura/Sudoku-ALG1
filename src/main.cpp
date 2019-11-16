#include "sudoku.hpp"

#include <iostream>

int main(int argc, char const *argv[]) {
    // std::cout << "Sudoku - TP3" << std::endl;

    Sudoku S = Sudoku(4, 2, 2);

    // S.print_adjacency();
    S.solve();
    // S.print_sudoku();
    return 0;
}
