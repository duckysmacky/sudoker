#ifndef SUDOKU_FIELD_HPP
#define SUDOKU_FIELD_HPP

#include <set>
#include <array>

class SudokuField
{
public:
    // The sudoku grid
    int m_grid[9][9];

private:
    // Size of the field (NxN)
    const int m_size;
    // Coordinates of pre-set numbers
    std::set<std::array<int, 2>> m_filled_numbers_coordinates;

public:
    SudokuField(int size = 9);

    // Fill a sudoku grid with numbers
    void fill_grid(int numbers_to_fill = 21);

    // Checks the grid if the number can be placed at the given coordinates
    bool can_place(int number, int nx, int ny) const;

    // Cheks if the number at the specified coordinates is pre-set by 
    // the user (cannot modify)
    bool is_preset(int x, int y) const;

    // Outputs the grid in a pretty way
    void print_grid() const;

private:
    // Fills a 3x3 area from one of the 9 sections of the Sudoku grid
    void fill_section(int* section, int x, int y) const;

    // Checks if the number doesn't appear in any of the groups to be considered unique
    // Each group is a vertical line, horizontal line and one of the 3x3 sections
    bool is_unique(int number, int* groups[3]) const;

    // Returns a random integer between max and min inclusively
    int randint(int min, int max) const;
};

#endif