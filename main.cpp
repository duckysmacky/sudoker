#include <iostream>

int randint(int min, int max);

class SudokuGame {
private:
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
        if (grid[ny][nx] != 0) return false;
        if (nx < 0 || ny < 0) return false;

        // Iterate over the grid and get the horizontal and vertical axis from the target coordinates
        int h_line[9] = { 0 };
        for (int i = 0; i < std::size(grid[ny]); i++) h_line[i] = grid[ny][i];

        int v_line[9] = { 0 };
        int i = 0;
        for (int y = 0; y < std::size(grid); y++)
        {
            for (int x = 0; x < std::size(grid[y]); x++)
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

public:

    SudokuGame(int fill_numbers = 21)
    {
        std::cout << "Filling the grid with " << fill_numbers << " numbers\n";

        while (fill_numbers > 0)
        {
            int x = randint(0, 8), y = randint(0, 8);
            int number = randint(1, 9);

            if (can_place(number, x, y))
            {
                grid[y][x] = number;
                fill_numbers--;
            }
        }

        std::cout << "Finished creating a new grid!\n";
    }

    // Outputs the grid in a pretty way
    void print_grid() const
    {
        for (int y = 0; y < std::size(grid); y++)
        {
            if (y % 3 == 0)
            {
                for (int i = 0; i < 25; i++) std::cout << "-";
                std::cout << "\n";
            }

            for (int x = 0; x < std::size(grid[y]); x++)
            {
                if (x % 3 == 0) std::cout << "| ";
                std::cout << grid[y][x] << " ";
                if (x == 8) std::cout << "|\n";
            }
        }
        for (int i = 0; i < 25; i++) std::cout << "-";
        std::cout << std::endl;
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
    return rand() % (abs(max - min) + 1) + min;
}

int main()
{
    SudokuGame game;
    game.be_verbose(true);
    game.print_grid();
}