#include "SudokuField.hpp"

#include <set>
#include <array>
#include <iostream>

SudokuField::SudokuField()
    : m_grid{ 0 }
{
    srand(time(0));
}

void SudokuField::fill_grid(int numbers_to_fill)
{
    std::cout << "Filling the grid with " << numbers_to_fill << " numbers\n";

    while (numbers_to_fill > 0)
    {
        int x = randint(0, 8), y = randint(0, 8);
        int number = randint(1, 9);

        if (can_place(number, y, x) && m_grid[y][x] == 0)
        {
            m_grid[y][x] = number;
            m_preset_coordinates.insert({ y, x });
            numbers_to_fill--;
        }
    }

    std::cout << "Finished creating a new grid!\n";
}

bool SudokuField::can_place(int number, int row, int column) const
{
    if (row < 0 || row >= FIELD_SIZE || column < 0 || column >= FIELD_SIZE) return false;

    // check the vertical line
    for (int y = 0; y < FIELD_SIZE; y++) {
        if (y != row && m_grid[y][column] == number) return false;
    }

    // check the horizontal line
    for (int x = 0; x < FIELD_SIZE; x++) {
        if (x != column && m_grid[row][x] == number) return false;
    }

    // check the 3x3 section
    int section_row = 3 * (row / 3);
    int section_column = 3 * (column / 3);
    for (int y = section_row; y < section_row + 3; y++) {
        for (int x = section_column; x < section_column + 3; x++) {
            if (y != row && x != column && m_grid[y][x] == number) return false;
        }
    }

    return true;
}

bool SudokuField::is_preset(int row, int column) const
{
    return m_preset_coordinates.find({ row, column }) != m_preset_coordinates.end();
}

void SudokuField::print_grid() const
{
    std::cout << "\033[H";

    for (int y = 0; y < FIELD_SIZE; y++)
    {
        if (y % 3 == 0)
        {
            for (int i = 0; i < 25; i++) std::cout << "-";
            std::cout << "\n";
        }

        for (int x = 0; x < FIELD_SIZE; x++)
        {
            if (x % 3 == 0) std::cout << "| ";
            std::cout << m_grid[y][x] << " ";
            if (x == 8) std::cout << "|\n";
        }
    }

    for (int i = 0; i < 25; i++)
        std::cout << "-";

    std::cout << std::endl;
}

int SudokuField::randint(int min, int max) const
{
    return rand() % (abs(max - min) + 1) + min;
}