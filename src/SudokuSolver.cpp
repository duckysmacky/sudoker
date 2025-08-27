#include "SudokuSolver.hpp"

#include <iostream>
#include <thread>
#include <chrono>

SudokuSolver::SudokuSolver(SudokuField* field)
    : m_field(field), m_be_verbose(false), m_hide_grid(false)
{
}

void SudokuSolver::solve()
{
    int x = 0, y = 0;
    int iteration = 0;
    bool result = true;

    std::cout << "Started solving..." << std::endl;
    while (true)
    {
        if (!m_hide_grid && iteration % 500 == 0) m_field->print_grid();

        // check if not out of bounds
        if (x < 0)
        {
            // move up the line
            if (y > 0)
            {
                y--;
                x = 8;
                continue;
            }
            else
            {
                std::cerr << "Error: index out of bounds! Exiting\n";
                break;
            }
        }

        // check if the current coordinates are not for filled numbers
        if (!m_field->is_preset(y, x))
        {
            if (m_be_verbose) std::cout << "Skipping number at (" << y << "; " << x << ")\n";
            if (result) x++;
            else x--;
            continue;
        }

        // move down the line
        if (x == 9)
        {
            x = 0;
            y++;
            continue;
        }

        // finish solving
        if (y == 9)
        {
            m_field->print_grid();
            std::cout << "\nDone!" << std::endl;
            break;
        }

        // we increment the number each time we change a position
        int num = m_field->m_grid[y][x] + 1;

        // move back when already tried every number at given position
        if (num > 9)
        {
            if (m_be_verbose) std::cout << "All numbers were tried for (" << y << "; " << x << ")! Going back\n";
            m_field->m_grid[y][x] = 0;
            x--;
            continue;
        }

        // increment the number until can be placed
        // if tried everything, mark as failed and go back later
        while (!(result = m_field->can_place(num, y, x)))
        {
            if (num < 9)
            {
                num++;
                if (m_be_verbose) std::cout << "Trying number " << num << " at (" << y << "; " << x << ")\n";
                continue;
            }
            else
            {
                break;
            }
        }

        // if was able to find a suitable number set it at the current coodniates, else go back and set current to zero
        if (result)
        {
            m_field->m_grid[y][x] = num;
            if (m_be_verbose) std::cout << "Found number " << num << " at (" << y << "; " << x << ")!\n";
            x++;
        }
        else
        {
            m_field->m_grid[y][x] = 0;
            if (m_be_verbose) std::cout << "Failed to placing anything at (" << y << "; " << x << ")! Going back\n";
            x--;
        }

        if (!m_hide_grid && iteration % 500 == 0) m_field->print_grid();

        iteration++;
    }
}

void SudokuSolver::be_verbose(bool value)
{
    m_be_verbose = value;
}

void SudokuSolver::hide_grid(bool value)
{
    m_hide_grid = value;
}