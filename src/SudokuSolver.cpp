#include "SudokuSolver.hpp"

#include <iostream>
#include <thread>
#include <chrono>

#include "SudokuSolver.hpp"

SudokuSolver::SudokuSolver(SudokuField* field)
    : m_field(field), m_be_verbose(false), m_show_grid(true)
{
}

void SudokuSolver::solve(int delay_ms)
{
    int x = 0, y = 0;
    bool result = true;

    std::cout << "Started solving..." << std::endl;
    while (true)
    {
        if (delay_ms != 0) std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));

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
                std::cout << "Error: index out of bounds! Exiting\n";
                break;
            }
        }

        // check if the current coordinates are not for filled numbers
        if (!m_field->is_preset(x, y))
        {
            if (m_show_grid) m_field->print_grid();
            if (m_be_verbose) std::cout << "Skipping number at (" << x << "; " << y << ")\n";
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
            std::cout << "Done!\n";
            break;
        }

        // we increment the number each time we change a position
        int num = m_field->m_grid[y][x] + 1;

        // move back when already tried every number at given position
        if (num > 9)
        {
            if (m_show_grid) m_field->print_grid();
            if (m_be_verbose) std::cout << "All numbers were tried for (" << x << "; " << y << ")! Going back\n";
            m_field->m_grid[y][x] = 0;
            x--;
            continue;
        }

        // increment the number until can be placed
        // if tried everything, mark as failed and go back later
        while (!(result = m_field->can_place(num, x, y)))
        {
            if (num < 9)
            {
                num++;
                if (m_be_verbose) std::cout << "Trying number " << num << " at (" << x << "; " << y << ")\n";
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
            if (m_show_grid) m_field->print_grid();
            if (m_be_verbose) std::cout << "Found number " << num << " at (" << x << "; " << y << ")!\n";
            x++;
        }
        else
        {
            m_field->m_grid[y][x] = 0;
            if (m_show_grid) m_field->print_grid();
            if (m_be_verbose) std::cout << "Failed to placing anything at (" << x << "; " << y << ")! Going back\n";
            x--;
        }
    }
}

void SudokuSolver::be_verbose(bool value)
{
    m_be_verbose = value;
}

void SudokuSolver::show_grid(bool value)
{
    m_show_grid = value;
}