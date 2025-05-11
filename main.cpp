#include <iostream>
#include <set>
#include <array>
#include <vector>
#include <string>
#include <chrono>
#include <thread>

static int randint(int min, int max);

class SudokuGame {
private:
    const int m_size;
    int m_grid[9][9];
    bool m_be_verbose;
    bool m_show_grid;
    std::set<std::array<int, 2>> filled_numbers_coordinates;

public:
    SudokuGame(int size = 9)
        : m_size(size), m_grid{ 0 }, m_be_verbose(false), m_show_grid(true)
    {
    }

    // Fill a sudoku grid with numbers
    void fill_grid(int numbers_to_fill = 21)
    {
        std::cout << "Filling the grid with " << numbers_to_fill << " numbers\n";

        while (numbers_to_fill > 0)
        {
            int x = randint(0, 8), y = randint(0, 8);
            int number = randint(1, 9);

            if (can_place(number, x, y))
            {
                m_grid[y][x] = number;
                filled_numbers_coordinates.insert({ x, y });
                numbers_to_fill--;
            }
        }

        std::cout << "Finished creating a new grid!\n";
    }

    // Outputs the grid in a pretty way
    void print_grid() const
    {
        system("cls"); // slows down the whole program, looking for alternatives to draw the grid
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
        for (int i = 0; i < 25; i++) std::cout << "-";
        std::cout << std::endl;
    }

    // Solves the current Sudoku grid by brute-forcing every position
    // Goes from left to right, top to bottom while trying to place numbers until they fit it
    void solve(int delay_ms = 1)
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
            if (filled_numbers_coordinates.find({ x, y }) != filled_numbers_coordinates.end())
            {
                if (m_show_grid) print_grid();
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
                print_grid();
                std::cout << "Done!\n";
                break;
            }

            // we increment the number each time we change a position
            int num = m_grid[y][x] + 1;

            // move back when already tried every number at given position
            if (num > 9)
            {
                if (m_show_grid) print_grid();
                if (m_be_verbose) std::cout << "All numbers were tried for (" << x << "; " << y << ")! Going back\n";
                m_grid[y][x] = 0;
                x--;
                continue;
            }

            // increment the number until can be placed
            // if tried everything, mark as failed and go back later
            while (!(result = can_place(num, x, y)))
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
                m_grid[y][x] = num;
                if (m_show_grid) print_grid();
                if (m_be_verbose) std::cout << "Found number " << num << " at (" << x << "; " << y << ")!\n";
                x++;
            }
            else
            {
                m_grid[y][x] = 0;
                if (m_show_grid) print_grid();
                if (m_be_verbose) std::cout << "Failed to placing anything at (" << x << "; " << y << ")! Going back\n";
                x--;
            }
        }
    }

    // Toggle verbose text output
    void be_verbose(bool value)
    {
        m_be_verbose = value;
    }

    // Toggle constant field output
    void show_grid(bool value)
    {
        m_show_grid = value;
    }

private:
    // Checks if the number doesn't appear in any of the groups to be considered unique
    // Each group is a vertical line, horizontal line and one of the 3x3 sections
    bool is_unique(int number, int* groups[3])
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

    // Fills a 3x3 area from one of the 9 sections of the Sudoku grid
    void fill_section(int* section, int x, int y) const
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

    // Checks the grid if the number can be placed at the given coordinates
    bool can_place(int number, int nx, int ny)
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

};

// Returns a random integer between max and min inclusively
static int randint(int min, int max)
{
    return rand() % (abs(max - min) + 1) + min;
}

// Takes arguments
static void parse_args(const std::vector<std::string>& args, SudokuGame& game)
{
    bool filled_grid = false;

    for (int i = 0; i < args.size(); i++)
    {
        // print help
        if (args[i] == "-h")
        {
            std::cout << " -v\t\tbe verbose\n -g\t\thide grid\n -n <count>\tnumbers to fill" << std::endl;
            exit(0);
        }

        // numbers to fill
        if (args[i] == "-n")
        {
            int count = std::stoi(args[i + 1]);
            game.fill_grid(count);
            filled_grid = true;
        }

        // be verbose
        if (args[i] == "-v")
            game.be_verbose(true);

        // hide grid
        if (args[i] == "-g")
            game.show_grid(false);
    }

    if (!filled_grid)
        game.fill_grid();
}

int main(int argc, char* argv[])
{
    // init random
    srand(time(0));

    std::vector<std::string> args;
    args.reserve(argc - 1);
    for (int i = 1; i < argc; i++) args.push_back(argv[i]);

    SudokuGame game = SudokuGame();
    parse_args(args, game);

    game.print_grid();
    std::cout << "Ready to solve. Press ENTER to start solving" << std::endl;
    std::cin.get();

    // record time taken
    auto time_start = std::chrono::high_resolution_clock::now();
    game.solve();
    auto time_end = std::chrono::high_resolution_clock::now();

    auto time_taken = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start);
    std::cout << "Time taken: " << time_taken.count() << "ms" << std::endl;
    return 0;
}
