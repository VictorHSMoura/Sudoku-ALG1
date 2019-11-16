#ifndef SUDOKU_HPP
#define SUDOKU_HPP

#include <vector>

class Sudoku {
private:
    int order, columns_per_block, lines_per_block;
    std::vector<int> *sudoku_adjacency;
    int *sudoku_values;
    int *saturation;

    void connect_lines_and_columns();
    void connect_blocks();
public:
    Sudoku(int order, int columns_per_block, int lines_per_block);
    ~Sudoku();
    void print_adjacency();
    bool solve();
    void print_sudoku();
};
#endif