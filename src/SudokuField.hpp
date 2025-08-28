#ifndef SUDOKU_FIELD_HPP
#define SUDOKU_FIELD_HPP

#include <set>
#include <array>

constexpr int FIELD_SIZE = 9;

class SudokuField
{
public:
    // The sudoku grid
    int m_grid[9][9];

private:
    // Coordinates of pre-set numbers
    std::set<std::array<int, 2>> m_preset_coordinates;

public:
    SudokuField();

    // Fill a sudoku grid with numbers
    void fill_grid(int numbers_to_fill);

    // Checks the grid if the number can be placed at the given coordinates
    bool can_place(int number, int row, int column) const;

    // Cheks if the number at the specified coordinates is pre-set by 
    // the user (cannot modify)
    bool is_preset(int row, int column) const;

    // Outputs the grid in a pretty way
    void print_grid() const;

private:
    // Returns a random integer between max and min inclusively
    int randint(int min, int max) const;
};

#endif