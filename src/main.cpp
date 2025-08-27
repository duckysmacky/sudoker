#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <chrono>

#include "SudokuField.hpp"
#include "SudokuSolver.hpp"

// Parses command line arguments and applies needed options to field and solver
static void parse_args(const std::vector<std::string_view>& args, SudokuField& field, SudokuSolver& solver)
{
    int fill_count = 21;

    for (int i = 0; i < args.size(); i++)
    {
        // print help
        if (args[i] == "--help" || args[i] == "-h")
        {
            std::cout
                << " --verbose, -v         \t\toutput extra information\n"
                << " --no-grid, -g         \t\tdon't show the grid\n"
                << " --numbers, -n <count> \t\tspecify how many numbers to fill in the grid\n"
                << std::flush;
            exit(0);
        }

        // numbers to fill
        if (args[i] == "--numbers" || args[i] == "-n")
        {
            if (i >= args.size() - 1)
            {
                std::cerr << "A required argument 'count' for the '--numbers' ('-n') flag was not provided!" << std::endl;
                exit(1);
            }

            int count = std::stoi(args[i + 1].data());

            if (count < 0 || count > 81)
            {
                std::cerr << "Invalid number count provided. Valid count range is [0; 81]" << std::endl;
                exit(1);
            }

            fill_count = count;
        }

        // be verbose
        if (args[i] == "--verbose" || args[i] == "-v")
            solver.be_verbose(true);

        // hide grid
        if (args[i] == "--no-grid" || args[i] == "-g")
            solver.hide_grid(true);
    }

    field.fill_grid(fill_count);
}

int main(int argc, char* argv[])
{
    std::vector<std::string_view> args;
    args.reserve(argc - 1);
    for (int i = 1; i < argc; i++)
        args.push_back(argv[i]);

    SudokuField field = SudokuField();
    SudokuSolver solver = SudokuSolver(field);
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