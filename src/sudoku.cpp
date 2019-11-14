#include "sudoku.hpp"

#include <iostream>

Sudoku::Sudoku(int order, int columns_per_block, int lines_per_block) {
    this->order = order;
    this->columns_per_block = columns_per_block;
    this->lines_per_block = lines_per_block;

    this->sudoku_adjacency = new std::vector<int>[order * order];

    this->connect_lines_and_columns();
    this->connect_blocks();
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
        for (std::vector<int>::iterator it = this->sudoku_adjacency[i].begin() ; it != this->sudoku_adjacency[i].end(); ++it)
            std::cout << *it <<  " ";
        std::cout <<std::endl;
    }
}