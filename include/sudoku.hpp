#ifndef SUDOKU_HPP
#define SUDOKU_HPP

#include <vector>
#include <set>

class Sudoku {
private:
    int order, columns_per_block, lines_per_block, sudoku_size;
    std::vector<int> *sudoku_adjacency;
    std::set<int> *cant_be;
    int *sudoku_values;

    void connect_lines_and_columns();
    void connect_blocks();
    bool is_complete();
    int find_max_probable_vertex();
    bool is_color_possible(int vertex, int color);
    void mark_adjacent_vertexes(int vertex, int color);
public:
    Sudoku(int order, int columns_per_block, int lines_per_block);
    ~Sudoku();
    void add_value(int row, int column, int value);
    void print_adjacency();
    bool solve();
    void print_sudoku();
};
#endif