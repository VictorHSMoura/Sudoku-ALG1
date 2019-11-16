#include "sudoku.hpp"

#include <iostream>

Sudoku::Sudoku(int order, int columns_per_block, int lines_per_block) {
    this->order = order;
    this->columns_per_block = columns_per_block;
    this->lines_per_block = lines_per_block;

    this->sudoku_adjacency = new std::vector<int>[order * order];
    this->sudoku_values = new int[order * order](); //initializing with 0's
    this->saturation = new int[order * order](); //initializing with 0's

    this->connect_lines_and_columns();
    this->connect_blocks();

    // TODO: implement a function to read the sudoku and set the values and saturation
    
    // setting some test values
    this->sudoku_values[1] = 4; this->sudoku_values[3] = 1;
    this->sudoku_values[4] = 3; this->sudoku_values[11] = 4;
    
    this->saturation[0] = 3; this->saturation[1] = 2; this->saturation[2] = 2;
    this->saturation[3] = 2; this->saturation[4] = 1; this->saturation[5] = 2;
    this->saturation[6] = 2; this->saturation[7] = 3; this->saturation[8] = 2;
    this->saturation[9] = 2; this->saturation[10] = 1; this->saturation[11] = 1;
    this->saturation[12] = 1; this->saturation[13] = 1; this->saturation[14] = 1;
    this->saturation[15] = 2;
}

Sudoku::~Sudoku() {    
    delete[] this->sudoku_adjacency;
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
    for (int i = 0; i < sudoku_size; i++) {
        for (std::vector<int>::iterator it = this->sudoku_adjacency[i].begin() ; it != this->sudoku_adjacency[i].end(); it++)
            std::cout << *it <<  " ";
        std::cout <<std::endl;
    }
}

bool Sudoku::solve() {
    // Consider a vertex with the highest degree of saturation. 
    // Break ties by considering that vertex with the highest degree. (same degree for all. Unnecessary)
    // Further ties are broken randomly.
    // Loop through the colour classes created so far, and colour the selected vertex with the first suitable colour.
    // Unless V is all coloured, return to step 1.
    int sudoku_size = this->order * this->order;
    int max_saturation_vertex, max_saturation;
    int possible_solution, sudoku_complete;

    while(true) {
        this->print_sudoku();
        std::cout << std::endl;

        max_saturation_vertex = 0;
        max_saturation = -1;
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

        // The existence of the solution depends on the way
        // the loop goes (ascending or descending).
        // Some solutions are found going from 0 to sudoku_size
        // and some are found going from sudoku_size to 0. There's no guarantee
        // to found a solution even looping these two ways

        // TODO: implement two way loop. Search for the solution
        // looping in a crescent order and in a decrescent order
        for (int i = sudoku_size - 1; i >= 0; i--) {
            if (this->saturation[i] > max_saturation && this->sudoku_values[i] == 0) {
                max_saturation_vertex = i;
                max_saturation = this->saturation[i];
            }
        }
        for (int i = 1; i <= this->order; i++) {
            int colour_possible = 1;
            for (std::vector<int>::iterator it = this->sudoku_adjacency[max_saturation_vertex].begin();
                it != this->sudoku_adjacency[max_saturation_vertex].end(); it++) {
                if(this->sudoku_values[*it] == i) {
                    colour_possible = 0;
                    break;
                }
            }
            if (colour_possible) {
                possible_solution = 1;
                this->sudoku_values[max_saturation_vertex] = i;

                // increase the saturation for adjacent vertexes
                for (std::vector<int>::iterator it = this->sudoku_adjacency[max_saturation_vertex].begin();
                it != this->sudoku_adjacency[max_saturation_vertex].end(); it++) {
                    this->saturation[*it]++;
                }
                break; // break the colour loop
            }
        }

        // if no colour was found for a vertex, then there's no solution for this algorithm
        // and we return that the solution wasn't found
        if(possible_solution == 0)
            return false;
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