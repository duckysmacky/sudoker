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
            m_filled_numbers_coordinates.insert({ y, x });
            numbers_to_fill--;
        }
    }

    std::cout << "Finished creating a new grid!\n";
}

bool SudokuField::can_place(int number, int row, int column) const
{
    if (row < 0 || row >= FIELD_SIZE || column < 0 || column >= FIELD_SIZE) return false;

    int h_line[FIELD_SIZE] = { 0 };
    int v_line[FIELD_SIZE] = { 0 };
    int section[FIELD_SIZE] = { 0 };

    // Iterate over the grid and get the horizontal and vertical axis from the target coordinates
    for (int i = 0; i < FIELD_SIZE; i++)
        h_line[i] = m_grid[row][i];

    for (int i = 0; i < FIELD_SIZE; i++)
        v_line[i] = m_grid[i][column];

    fill_section(section, row, column);

    int* groups[3] = { h_line, v_line, section };
    return is_unique(number, groups);
}

bool SudokuField::is_preset(int row, int column) const
{
    return m_filled_numbers_coordinates.find({ row, column }) == m_filled_numbers_coordinates.end();
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

void SudokuField::fill_section(int* section, int row, int column) const
{
    int section_y = 3 * (row / 3);
    int section_x = 3 * (column / 3);

    int i = 0;
    for (int y = section_y; y < section_y + 3; y++)
    {
        for (int x = section_x; x < section_x + 3; x++)
        {
            section[i] = m_grid[y][x];
            i++;
        }
    }
}

bool SudokuField::is_unique(int number, int* groups[3]) const
{
    for (int g = 0; g < 3; g++)
    {
        for (int i = 0; i < FIELD_SIZE; i++)
        {
            if (groups[g][i] == number) return false;
        }
    }
    return true;
}

int SudokuField::randint(int min, int max) const
{
    return rand() % (abs(max - min) + 1) + min;
}