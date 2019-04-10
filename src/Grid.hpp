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
    const Cell& GetCell(Position const& position) const;

    int GetGridSize() const;
    int GetBlockSize() const;

private:
    std::vector<Cell> m_Cells;

    const int m_GridSize;
    const int m_BlockSize;
};

std::ostream& operator<<(std::ostream& os, Grid const& grid);

bool operator==(const Grid& lhs, const Grid& rhs);

} // namespace sudoku
