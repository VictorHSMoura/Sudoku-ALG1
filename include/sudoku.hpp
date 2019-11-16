#ifndef SUDOKU_HPP
#define SUDOKU_HPP

#include <vector>
#include <set>

class Sudoku {
private:
    int order, columns_per_block, lines_per_block;
    std::vector<int> *sudoku_adjacency;
    std::set<int> *cant_be;
    int *sudoku_values;
    int *saturation;

    void connect_lines_and_columns();
    void connect_blocks();
    int find_max_saturation_vertex(int option);
public:
    Sudoku(int order, int columns_per_block, int lines_per_block);
    ~Sudoku();
    void add_value(int row, int column, int value);
    void print_adjacency();
    bool solve();
    void print_sudoku();
};
#endif