#include "sudoku.hpp"

#include <iostream>

Sudoku::Sudoku(int order, int columns_per_block, int lines_per_block) {
    this->order = order;
    this->columns_per_block = columns_per_block;
    this->lines_per_block = lines_per_block;
    this->sudoku_size = order * order;

    this->sudoku_adjacency = new std::vector<int>[this->sudoku_size];
    this->cant_be = new std::set<int>[this->sudoku_size];
    this->sudoku_values = new int[this->sudoku_size];

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
    
    if(value != 0)
        for (it = begin; it != end; it++)
            this->cant_be[*it].insert(value);
}

void Sudoku::connect_lines_and_columns() {
    for (int i = 0; i < this->order; i++) {
        for (int j = 0; j < this->order; j++) {
            int position = i * this->order + j;
            //connect lines
            for (int column = j + 1; column < this->order; column++) {
                this->sudoku_adjacency[position].push_back(i * this->order + column);
                this->sudoku_adjacency[i * this->order + column].push_back(position);
            }

            //connect columns
            for (int line = i + 1; line < this->order; line++) {
                this->sudoku_adjacency[position].push_back(line * this->order + j);
                this->sudoku_adjacency[line * this->order + j].push_back(position);
            }
        }
    }
}

void Sudoku::connect_blocks() {
    for (int i = 0; i < this->order; i++) {
        for (int j = 0; j < this->order; j++) {
            int source = i * this->order + j;
            for(int line = i + 1; (line % this->lines_per_block != 0); line++) {
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
    std::vector<int>::iterator it, begin, end;

    for (int i = 0; i < this->sudoku_size; i++) {
        begin = this->sudoku_adjacency[i].begin();
        end = this->sudoku_adjacency[i].end();
        
        for (it = begin; it != end; it++)
            std::cout << *it <<  " ";
        std::cout <<std::endl;
    }
}

bool Sudoku::is_complete() {
    for (int i = 0; i < this->sudoku_size; i++)
        if(this->sudoku_values[i] == 0)
            return false;
    return true;
}

int Sudoku::find_max_probable_vertex() {
    int max_probable_vertex = 0, max_probable = 0;
    for (int i = 0; i < this->sudoku_size; i++) {
        if (this->cant_be[i].size() > max_probable && this->sudoku_values[i] == 0) {
            max_probable_vertex = i;
            max_probable = this->cant_be[i].size();
        }
    }
    return max_probable_vertex;
}

bool Sudoku::is_color_possible(int vertex, int color) {
    return this->cant_be[vertex].find(color) == this->cant_be[vertex].end();
}

void Sudoku::mark_adjacent_vertexes(int vertex, int color) {
    std::vector<int>::iterator it, begin, end;
    begin = this->sudoku_adjacency[vertex].begin();
    end = this->sudoku_adjacency[vertex].end();

    for (it = begin; it != end; ++it) {
        this->cant_be[*it].insert(color);
    }
}

bool Sudoku::solve() {
    // Consider a vertex with the greatest "can't be" set
    // Break ties by considering that vertex with the highest degree. (same degree for all. Unnecessary)
    // Further ties are broken randomly.
    // Loop through the color classes created so far, and color the selected vertex with the first suitable color.
    // Unless V is all colored, return to step 1.
    int max_probable_vertex;
    bool solution_possible;

    while(true) {
        solution_possible = false;

        // if the sudoku is complete, then we return that we found the solution
        if(this->is_complete())
            return true;  

        max_probable_vertex = this->find_max_probable_vertex();

        for (int color = 1; color <= this->order; color++) {
            int color_possible = this->is_color_possible(max_probable_vertex, color);
            
            if (color_possible) {
                solution_possible = true;
                this->sudoku_values[max_probable_vertex] = color;

                // add the color to the can't be set of the adjacent vertexes
                this->mark_adjacent_vertexes(max_probable_vertex, color);
                break; // break the color loop
            }
        }

        // if no color was found for a vertex, then there's no solution for this algorithm
        // and we return that the solution wasn't found
        if(!solution_possible){
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