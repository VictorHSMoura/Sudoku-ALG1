#include "sudoku.hpp"

#include <iostream>
#include <fstream>
#include <cstdlib>

int main(int argc, char const *argv[]) {
    int n, i, j, value;
    std::ifstream input_file;

    if(argc <= 1) exit(1);

    input_file.open(argv[1]);
    if (input_file.is_open()) {
        input_file >> n >> i >> j;
    
        Sudoku S = Sudoku(n, i, j);

        for (int row = 0; row < n; row++) {
            for (int column = 0; column < n; column++) {
                input_file >> value;
                S.add_value(row, column, value);
            }
        }
        
        std::cout << (S.solve() ? "solução" : "sem solução") << std::endl;

        S.print_sudoku();
    }
    return 0;
}
