#include "Grid.hpp"

#include <boost/range/irange.hpp>

namespace sudoku
{

namespace constants
{
const std::string VerticalSeparator = "|";
constexpr char EmptyCellChar = '*';
} // namespace constants

namespace
{

int GetBlockSize(int gridSize)
{
    auto blockSize = sqrt(gridSize);

    if (blockSize != (int)blockSize)
        throw std::runtime_error("Can't deduce block size from grid size '" + std::to_string(gridSize) + "'");

    return blockSize;
}

void PrintHorizontalLine(std::ostream& os, int gridSize, int blockSize)
{
    for (auto col : boost::irange(0, gridSize))
    {
        if (col % blockSize == 0)
            os << "+";

        os << "--";
    }

    os << "+" << std::endl;
}

void PrintVerticalSeparator(std::ostream& os)
{
    os << constants::VerticalSeparator;
}

void PrintCell(std::ostream& os, Cell const& cell)
{
    auto value = cell.GetValue();

    if (value)
        os << *value;
    else
        os << constants::EmptyCellChar;

    os << " ";
}

} // anonymous namespace

Grid::Grid(int gridSize) :
    m_Cells(boost::extents[gridSize][gridSize]),
    m_GridSize(gridSize),
    m_BlockSize(GetBlockSize(gridSize))
{
    if (gridSize < 4)
        throw std::runtime_error("Invalid Sudoku grid size '" + std::to_string(gridSize) + "', because: too small.");

    for(auto row : boost::irange(0, m_GridSize))
    {
        for(auto col : boost::irange(0, m_GridSize))
        {
            m_Cells[row][col] = std::make_shared<Cell>(Position{row, col}, gridSize);
        }
    }
}

Grid::Grid(Grid const& grid) :
    m_Cells(boost::extents[grid.m_GridSize][grid.m_GridSize]),
    m_GridSize(grid.m_GridSize),
    m_BlockSize(grid.m_BlockSize)
{
    for(auto row : boost::irange(0, grid.m_GridSize))
    {
        for(auto col : boost::irange(0, grid.m_GridSize))
        {
            m_Cells[row][col] = std::make_shared<Cell>(*grid.m_Cells[row][col]);
        }
    }
}

Grid& Grid::operator=(Grid const& grid)
{
    for(auto row : boost::irange(0, grid.m_GridSize))
    {
        for(auto col : boost::irange(0, grid.m_GridSize))
        {
            *m_Cells[row][col] = *grid.m_Cells[row][col];
        }
    }

    return *this;
}

SharedCell Grid::GetCell(Position const& position)
{
    return m_Cells[position.m_Row][position.m_Col];
}

std::ostream& operator<<(std::ostream& os, Grid const& grid)
{
    for(auto row : boost::irange(0, grid.m_GridSize))
    {
        if (row % grid.m_BlockSize == 0)
            PrintHorizontalLine(os, grid.m_GridSize, grid.m_BlockSize);

        for(auto col : boost::irange(0, grid.m_GridSize))
        {
            if (col % grid.m_BlockSize == 0)
                PrintVerticalSeparator(os);

            PrintCell(os, *grid.m_Cells[row][col]);
        }

        PrintVerticalSeparator(os);
        os << std::endl;
    }

    PrintHorizontalLine(os, grid.m_GridSize, grid.m_BlockSize);

    return os;
}

bool operator==(const Grid& lhs, const Grid& rhs)
{
    return lhs.m_GridSize == rhs.m_GridSize
            && std::equal(lhs.begin(), lhs.end(), rhs.begin(), [](auto const& lhs, auto const& rhs){ return *lhs == *rhs; });
}

} // namespace sudoku
