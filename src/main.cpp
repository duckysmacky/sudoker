#include <iostream>
#include <vector>
#include <string>
#include <chrono>

#include "SudokuField.hpp"
#include "SudokuSolver.hpp"

// Parses command line arguments and applies needed options to field and solver
static void parse_args(const std::vector<std::string>& args, SudokuField& field, SudokuSolver& solver)
{
    bool filled_grid = false;

    for (int i = 0; i < args.size(); i++)
    {
        // print help
        if (args[i] == "-h")
        {
            std::cout
                << " -v\t\tbe verbose\n"
                << " -g\t\thide grid\n"
                << " -n <count>\tnumbers to fill"
                << std::endl;
            exit(0);
        }

        // numbers to fill
        if (args[i] == "-n")
        {
            int count = std::stoi(args[i + 1]);
            field.fill_grid(count);
            filled_grid = true;
        }

        // be verbose
        if (args[i] == "-v")
            solver.be_verbose(true);

        // hide grid
        if (args[i] == "-g")
            solver.show_grid(false);
    }

    if (!filled_grid)
        field.fill_grid();
}

int main(int argc, char* argv[])
{
    // init random
    srand(time(0));

    std::vector<std::string> args;
    args.reserve(argc - 1);
    for (int i = 1; i < argc; i++)
        args.push_back(argv[i]);

    SudokuField field = SudokuField();
    SudokuSolver solver = SudokuSolver(&field);
    parse_args(args, field, solver);

    system("clear");
    field.print_grid();
    std::cout << "Ready to solve. Press ENTER to start solving" << std::endl;
    std::cin.get();

    // record time taken
    auto time_start = std::chrono::high_resolution_clock::now();
    solver.solve();
    auto time_end = std::chrono::high_resolution_clock::now();

    auto time_taken = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start);
    std::cout << "Time taken: " << time_taken.count() << "ms" << std::endl;
    return 0;
}