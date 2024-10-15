#include <iostream>
#include <set>
#include <array>
#include <windows.h>

int randint(int min, int max);

class SudokuGame {
private:
    const int size = 9;
    int grid[9][9] = { 0 };
    bool verbose = false;

    // Checks if the number doesn't appear in any of the groups to be considered unique
    // Each group is a vertical line, horizontal line and one of the 3x3 sections
    bool is_unique(int number, int *groups[3])
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

    // Returns a 3x3 area from one of the 9 sections of the Sudoku grid
    int* get_section(int x, int y) const
    {
        int section[9] = { 0 };
        int section_x = 3 * (x / 3), section_y = 3 * (y / 3);
        
        int i = 0;
        for (int row = section_y; row < section_y + 3; row++)
        {
            for (int col = section_x; col < section_x + 3; col++)
            {
                section[i] = grid[row][col];
                i++;
            }
        }

        return section;
    }

    // Checks the grid if the number can be placed at the given coordinates
    bool can_place(int number, int nx, int ny)
    {
        if (nx < 0 || ny < 0) return false;

        // Iterate over the grid and get the horizontal and vertical axis from the target coordinates
        int h_line[9] = { 0 };
        for (int i = 0; i < size; i++) h_line[i] = grid[ny][i];
        int v_line[9] = { 0 };

        int i = 0;
        for (int y = 0; y < size; y++)
        {
            for (int x = 0; x < size; x++)
            {
                if (x == nx)
                {
                    v_line[i] = grid[y][x];
                    i++;
                }
            }
        }

        int *section = get_section(nx, ny);
        int *groups[3] = { h_line, v_line, section };
        return is_unique(number, groups);
    }

    // Finds the coordinates of the numbers which were originalled placed (filled on creation)
    std::set<std::array<int, 2>> find_filled_numbers() const
    {
        std::set<std::array<int, 2>> coordinates; // max possible numbers to fill is 81

        for (int y = 0; y < size; y++)
        {
            for (int x = 0; x < size; x++)
            {
                if (grid[y][x] != 0) coordinates.insert({ x, y });
            }
        }

        return coordinates;
    }

public:
    SudokuGame(int numbers_to_fill = 21)
    {
        std::cout << "Filling the grid with " << numbers_to_fill << " numbers\n";

        while (numbers_to_fill > 0)
        {
            int x = randint(0, 8), y = randint(0, 8);
            int number = randint(1, 9);

            if (can_place(number, x, y))
            {
                grid[y][x] = number;
                numbers_to_fill--;
            }
        }

        std::cout << "Finished creating a new grid!\n";
    }

    // Outputs the grid in a pretty way
    void print_grid() const
    {
        system("cls");
        for (int y = 0; y < size; y++)
        {
            if (y % 3 == 0)
            {
                for (int i = 0; i < 25; i++) std::cout << "-";
                std::cout << "\n";
            }

            for (int x = 0; x < size; x++)
            {
                if (x % 3 == 0) std::cout << "| ";
                std::cout << grid[y][x] << " ";
                if (x == 8) std::cout << "|\n";
            }
        }
        for (int i = 0; i < 25; i++) std::cout << "-";
        std::cout << std::endl;
    }

    void solve(int delay_ms = 100)
    {
        std::set<std::array<int, 2>> skip_coordinates = find_filled_numbers();
        int x = 0, y = 0;
        bool result = true;

        while (true)
        {
            if (delay_ms != 0) Sleep(delay_ms);

            // check if not out of bounds
            if (x < 0)
            {
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
            if (skip_coordinates.find({ x, y }) != skip_coordinates.end())
            {
                if (verbose) std::cout << "Skipping number at (" << x << "; " << y << ")\n";
                print_grid();
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
                std::cout << "Done!\n";
                break;
            }

            int num = grid[y][x] + 1;

            if (num > 9)
            {
                if (verbose) std::cout << "Number " << num - 1 << " at (" << x << "; " << y << ") is already max! Going back\n";
                print_grid();
                grid[y][x] = 0;
                x--;
                continue;
            }

            while (!(result = can_place(num, x, y)))
            {
                if (num < 9)
                {
                    num++;
                    if (verbose) std::cout << "Trying number " << num << " at (" << x << "; " << y << ")\n";
                    continue;
                }
                else
                {
                    break;
                }
            }

            if (result)
            {
                grid[y][x] = num;
                if (verbose) std::cout << "Found number " << num << " at (" << x << "; " << y << ")!\n";
                print_grid();
                x++;
            }
            else
            {
                grid[y][x] = 0;
                if (verbose) std::cout << "Failed to placing anything at (" << x << "; " << y << ")! Going back\n";
                print_grid();
                x--;
            }
        }
    }

    void solve_old()
    {
        std::set<std::array<int, 2>> skip_coordinates = find_filled_numbers();
        bool result = true;
        int x = 0, y = 0;

        while (y < size)
        {
            x = (x >= 0) ? 0 : 8;

            while (x < size)
            {
                // check if the current coordinates are not for filled numbers
                if (skip_coordinates.find({ x, y }) != skip_coordinates.end())
                {
                    if (result || x == 0) x++;
                    else if (x != 0) x--;
                }

                if (x == 8)
                {
                    x = 0;
                    y++;
                }

                int num = grid[y][x] + 1;
                while (!(result = can_place(num, x, y)))
                {
                    if (verbose) std::cout << "Trying number " << num << " at (" << x << "; " << y << ")\n";

                    if (num < 9) num++;
                    else
                    {
                        if (verbose) std::cout << "Failed to placing anything at (" << x << "; " << y << "). Going back\n";
                        num = 0;
                        grid[y][x] = 0;
                        print_grid();
                        x--;
                        break;
                    }
                }

                if (result)
                {
                    grid[y][x] = num;
                    if (verbose) std::cout << "Found number " << num << " at (" << x << "; " << y << ")!\n";
                    print_grid();
                    x++;
                }
            }

            if (x < 0 && y != 0)
            {
                y--;
                x = 8;
            }
            else y++;
        }
    }

    // Toggles verbose output for extra info
    void be_verbose(bool value)
    {
        verbose = value;
    }

};

// Returns a random integer between max and min inclusively
static int randint(int min, int max)
{
    //srand(time(0));
    return rand() % (abs(max - min) + 1) + min;
}

int main()
{
    SudokuGame game;
    game.be_verbose(false);
    game.print_grid();
    game.solve(10);
}