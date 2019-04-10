#include "GridLoader.hpp"

using namespace sudoku;

Grid GridLoader::GetHardcodedGrid() const
{
    Grid grid(9);

    grid.GetCell(Position{0, 3}).SetValue(9);
    grid.GetCell(Position{0, 5}).SetValue(2);
    grid.GetCell(Position{1, 1}).SetValue(4);
    grid.GetCell(Position{1, 7}).SetValue(5);
    grid.GetCell(Position{2, 2}).SetValue(2);
    grid.GetCell(Position{2, 6}).SetValue(3);
    grid.GetCell(Position{3, 0}).SetValue(2);
    grid.GetCell(Position{3, 8}).SetValue(7);
    grid.GetCell(Position{4, 3}).SetValue(4);
    grid.GetCell(Position{4, 4}).SetValue(5);
    grid.GetCell(Position{4, 5}).SetValue(6);
    grid.GetCell(Position{5, 0}).SetValue(6);
    grid.GetCell(Position{5, 8}).SetValue(9);
    grid.GetCell(Position{6, 2}).SetValue(7);
    grid.GetCell(Position{6, 6}).SetValue(8);
    grid.GetCell(Position{7, 1}).SetValue(3);
    grid.GetCell(Position{7, 7}).SetValue(4);
    grid.GetCell(Position{8, 3}).SetValue(2);
    grid.GetCell(Position{8, 5}).SetValue(7);

    return grid;
}
