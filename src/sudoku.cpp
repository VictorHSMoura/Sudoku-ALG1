#include "sudoku.hpp"

#include <iostream>

Sudoku::Sudoku(int order, int columns_per_block, int lines_per_block) {
    this->order = order;
    this->columns_per_block = columns_per_block;
    this->lines_per_block = lines_per_block;

    this->sudoku_adjacency = new std::vector<int>[order * order];
    this->cant_be = new std::set<int>[order * order];
    this->sudoku_values = new int[order * order];

    this->connect_lines_and_columns();
    this->connect_blocks();
}

Sudoku::~Sudoku() {    
    delete[] this->sudoku_adjacency;
    delete[] this->cant_be;
    delete[] this->sudoku_values;
}

void Sudoku::add_value(int row, int column, int value) {
    int position = row * this->order + column;
    std::vector<int>::iterator it, begin, end;
    
    begin = this->sudoku_adjacency[position].begin();
    end = this->sudoku_adjacency[position].end();
    
    this->sudoku_values[position] = value;
    
    if(value != 0) {
        for (it = begin; it != end; it++) {
            this->cant_be[*it].insert(value);
        }
    }
}

void Sudoku::connect_lines_and_columns() {
    int sudoku_size = this->order * this->order;

    for (int i = 0; i < sudoku_size; i++) {
        int line_i = (i / this->order);
        //connect lines
        for (int column = (i % this->order) + 1; column < this->order; column++) {
            this->sudoku_adjacency[i].push_back(line_i * this->order + column);
            this->sudoku_adjacency[line_i * this->order + column].push_back(i);
        }

        int column_i = (i % this->order);
        //connect columns
        for (int line = (i / this->order) + 1; line < this->order; line++) {
            this->sudoku_adjacency[i].push_back(line * this->order + column_i);
            this->sudoku_adjacency[line * this->order + column_i].push_back(i);
        }
    }
}

void Sudoku::connect_blocks() {
    for (int i = 0; i < this->order; i++) {
        for (int j = 0; j < this->order; j++) {
            int source = i * this->order + j;
            for(int line = i + 1; (line < this->order) && (line % this->lines_per_block != 0); line++) {
                for(int column = 0; column < this->columns_per_block; column++) {
                    //column correcting factor
                    //get the correct indices for the block
                    column += (j / this->columns_per_block) * this->columns_per_block;
                    int dest = line * this->order + column;
                    if(j % this->columns_per_block != column % this->columns_per_block) {
                        this->sudoku_adjacency[source].push_back(dest);
                        this->sudoku_adjacency[dest].push_back(source);
                    }
                    //undo the correction to keep the loop-for correct
                    column -= (j / this->columns_per_block) * this->columns_per_block;
                }
            }
        }
    }
}

void Sudoku::print_adjacency() {
    int sudoku_size = this->order * this->order;
    std::vector<int>::iterator it, begin, end;

    for (int i = 0; i < sudoku_size; i++) {
        begin = this->sudoku_adjacency[i].begin();
        end = this->sudoku_adjacency[i].end();
        
        for (it = begin; it != end; it++)
            std::cout << *it <<  " ";
        std::cout <<std::endl;
    }
}

bool Sudoku::solve() {
    // Consider a vertex with the greatest "can't be" set
    // Break ties by considering that vertex with the highest degree. (same degree for all. Unnecessary)
    // Further ties are broken randomly.
    // Loop through the colour classes created so far, and colour the selected vertex with the first suitable colour.
    // Unless V is all coloured, return to step 1.
    int sudoku_size = this->order * this->order;
    int max_probable_vertex, max_probable;
    int possible_solution, sudoku_complete;

    while(true) {

        max_probable_vertex = 0;
        max_probable = 0;
        possible_solution = 0;
        sudoku_complete = 1;

        for (int i = 0; i < sudoku_size; i++) {
            if(this->sudoku_values[i] == 0) {
                sudoku_complete = 0;
                break;
            }
        }

        // if the sudoku is complete, then we return that we found the solution
        if(sudoku_complete)
            return true;  

        for (int i = 0; i < sudoku_size; i++) {
            if (this->cant_be[i].size() > max_probable && this->sudoku_values[i] == 0) {
                max_probable_vertex = i;
                max_probable = this->cant_be[i].size();
            }
        }

        for (int i = 1; i <= this->order; i++) {
            int colour_possible = 1;

            if(this->cant_be[max_probable_vertex].find(i) != this->cant_be[max_probable_vertex].end()) {
                colour_possible = 0;
            }
            
            if (colour_possible) {
                possible_solution = 1;
                this->sudoku_values[max_probable_vertex] = i;

                // add the color to the can't be set of the adjacent vertexes
                for (std::vector<int>::iterator it = this->sudoku_adjacency[max_probable_vertex].begin();
                it != this->sudoku_adjacency[max_probable_vertex].end(); ++it) {
                    this->cant_be[*it].insert(i);
                }
                break; // break the colour loop
            }
        }

        // if no colour was found for a vertex, then there's no solution for this algorithm
        // and we return that the solution wasn't found
        if(possible_solution == 0){
            return false;
        }
    }
}

void Sudoku::print_sudoku() {
    for (int i = 0; i < this->order; i++) {
        for (int j = 0; j < this->order; j++) {
            std::cout << this->sudoku_values[i * this->order + j] << " ";
        }
        std::cout << std::endl;
    }
}