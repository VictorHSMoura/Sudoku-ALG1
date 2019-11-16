#include "sudoku.hpp"

#include <iostream>

int main(int argc, char const *argv[]) {
    // std::cout << "Sudoku - TP3" << std::endl;
    int n, i, j, value;
    std::cin >> n >> i >> j;
    
    Sudoku S = Sudoku(n, i, j);

    for (int row = 0; row < n; row++) {
        for (int column = 0; column < n; column++) {
            std::cin >> value;
            S.add_value(row, column, value);
        }
    }

    // S.print_adjacency();
    S.solve();
    S.print_sudoku();
    return 0;
}
