CC := g++
SRC := src/main.cpp src/SudokuField.cpp src/SudokuSolver.cpp

debug:
	$(CC) $(SRC) -o sudoker -g

release:
	$(CC) $(SRC) -o sudoker -O3
