#include "sudoku.hpp"

#include <iostream>

int main(int argc, char const *argv[]) {
    // std::cout << "Sudoku - TP3" << std::endl;

    Sudoku S = Sudoku(8, 4, 2);

    S.print_adjacency();
    return 0;
}
