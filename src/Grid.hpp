#pragma once

#include <ostream>

#include <boost/multi_array.hpp>

#include "Cell.hpp"

namespace sudoku
{

struct Grid
{
    Grid(int gridSize);

    auto begin() const { return m_Cells.origin(); }
    auto end() const { return m_Cells.origin() + m_Cells.num_elements(); }

    boost::multi_array<SharedCell, 2> m_Cells;

    const int m_GridSize;
    const int m_BlockSize;
};

std::ostream& operator<<(std::ostream& os, Grid const& grid);

} // namespace sudoku
