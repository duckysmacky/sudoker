#ifndef SUDOKU_SOLVER_HPP
#define SUDOKU_SOLVER_HPP

#include "SudokuField.hpp"

class SudokuSolver
{
private:
    // Pointer to the field to solve
    SudokuField* m_field;
    bool m_be_verbose;
    bool m_show_grid;

public:
    SudokuSolver(SudokuField* field);

    // Solves the current Sudoku grid by brute-forcing every position
    // Goes from left to right, top to bottom while trying to place numbers until they fit it
    void solve();

    // Toggle verbose text output
    void be_verbose(bool value);
    // Toggle constant field output
    void show_grid(bool value);
};

#endif