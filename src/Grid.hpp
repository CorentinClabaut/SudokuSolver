#pragma once

#include <ostream>

#include <boost/multi_array.hpp>

#include "Cell.hpp"

namespace sudoku
{

struct Grid
{
    Grid(int gridSize);
    Grid(Grid const& grid);

    Grid& operator=(Grid const& grid);

    auto begin() const { return m_Cells.origin(); }
    auto end() const { return m_Cells.origin() + m_Cells.num_elements(); }

    auto begin() { return m_Cells.origin(); }
    auto end() { return m_Cells.origin() + m_Cells.num_elements(); }

    SharedCell GetCell(Position const& position);

    boost::multi_array<SharedCell, 2> m_Cells;

    const int m_GridSize;
    const int m_BlockSize;
};

std::ostream& operator<<(std::ostream& os, Grid const& grid);

bool operator==(const Grid& lhs, const Grid& rhs);

} // namespace sudoku
