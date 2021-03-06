#pragma once

#include <ostream>
#include <vector>

#include "Cell.hpp"

namespace sudoku
{

class Grid
{
public:
    Grid(int gridSize);
    Grid(Grid const& grid);

    Grid& operator=(Grid const& grid);

    auto begin() const { return m_Cells.begin(); }
    auto end() const { return m_Cells.end(); }

    auto begin() { return m_Cells.begin(); }
    auto end() { return m_Cells.end(); }

    Cell& GetCell(Position const& position);
    Cell const& GetCell(Position const& position) const;

    int GetGridSize() const;

private:
    std::vector<Cell> m_Cells;

    const int m_GridSize;
};

std::ostream& operator<<(std::ostream& os, Grid const& grid);

bool operator==(Grid const& lhs, Grid const& rhs);

} // namespace sudoku
