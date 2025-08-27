#include "SudokuField.hpp"

#include <set>
#include <array>
#include <iostream>

SudokuField::SudokuField(int size)
    : m_size(size), m_grid{ 0 }
{
}

void SudokuField::fill_grid(int numbers_to_fill)
{
    std::cout << "Filling the grid with " << numbers_to_fill << " numbers\n";

    while (numbers_to_fill > 0)
    {
        int x = randint(0, 8), y = randint(0, 8);
        int number = randint(1, 9);

        if (can_place(number, x, y))
        {
            m_grid[y][x] = number;
            m_filled_numbers_coordinates.insert({ x, y });
            numbers_to_fill--;
        }
    }

    std::cout << "Finished creating a new grid!\n";
}

bool SudokuField::can_place(int number, int nx, int ny) const
{
    if (nx < 0 || ny < 0) return false;

    int h_line[9] = { 0 };
    int v_line[9] = { 0 };
    int section[9] = { 0 };

    // Iterate over the grid and get the horizontal and vertical axis from the target coordinates
    for (int i = 0; i < m_size; i++)
        h_line[i] = m_grid[ny][i];

    int i = 0;
    for (int y = 0; y < m_size; y++)
    {
        for (int x = 0; x < m_size; x++)
        {
            if (x == nx)
            {
                v_line[i] = m_grid[y][x];
                i++;
            }
        }
    }

    fill_section(section, nx, ny);

    int* groups[3] = { h_line, v_line, section };
    return is_unique(number, groups);
}

bool SudokuField::is_preset(int x, int y) const
{
    return m_filled_numbers_coordinates.find({ x, y }) == m_filled_numbers_coordinates.end();
}

void SudokuField::print_grid() const
{
    std::cout << "\033[H";

    for (int y = 0; y < m_size; y++)
    {
        if (y % 3 == 0)
        {
            for (int i = 0; i < 25; i++) std::cout << "-";
            std::cout << "\n";
        }

        for (int x = 0; x < m_size; x++)
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

void SudokuField::fill_section(int* section, int x, int y) const
{
    int section_x = 3 * (x / 3), section_y = 3 * (y / 3);

    int i = 0;
    for (int row = section_y; row < section_y + 3; row++)
    {
        for (int col = section_x; col < section_x + 3; col++)
        {
            section[i] = m_grid[row][col];
            i++;
        }
    }
}

bool SudokuField::is_unique(int number, int* groups[3]) const
{
    for (int g = 0; g < 3; g++)
    {
        for (int i = 0; i < 9; i++)
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