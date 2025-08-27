# Sudoku Solver

An efficient Sudoku grid generator and solver written in C++

![solved grid](https://i.imgur.com/3EMgI6O.png)

---

The algorithm works by going from left to right, top to bottom while trying to find matching numbers (0 to 9) until
they fit it, aka can be placed while following all of the Sudoku rules

Average solve time: \~2 seconds (depending on the generated grid)

### Planned features:

- [x] Grid generation
- [x] Grid visualization
- [x] Solving algorithm
- [x] Rendering optimisations
- [ ] Custom sudoku field import
- [ ] CLI overhaul (grid + input)
