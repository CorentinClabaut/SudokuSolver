#include "Grid.hpp"

#include <cmath>

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

int CalculateBlockSize(int gridSize)
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
    m_GridSize(gridSize),
    m_BlockSize(CalculateBlockSize(gridSize))
{
    m_Cells.reserve(gridSize * gridSize);

    if (gridSize < 4)
        throw std::runtime_error("Invalid Sudoku grid size '" + std::to_string(gridSize) + "', because: too small.");

    for(auto row : boost::irange(0, m_GridSize))
    {
        for(auto col : boost::irange(0, m_GridSize))
        {
            m_Cells.emplace_back(Position{row, col}, gridSize);
        }
    }
}

Grid::Grid(Grid const& grid) :
    m_GridSize(grid.GetGridSize()),
    m_BlockSize(grid.GetBlockSize())
{
    m_Cells.reserve(grid.GetGridSize() * grid.GetGridSize());

    std::copy(grid.begin(), grid.end(), std::back_inserter(m_Cells));
}

Grid& Grid::operator=(Grid const& grid)
{
    std::copy(grid.begin(), grid.end(), m_Cells.begin());

    return *this;
}

Cell& Grid::GetCell(Position const& position)
{
    return m_Cells.at(position.m_Row * m_GridSize + position.m_Col);
}

const Cell& Grid::GetCell(Position const& position) const
{
    return m_Cells.at(position.m_Row * m_GridSize + position.m_Col);
}

int Grid::GetGridSize() const
{
    return m_GridSize;
}

int Grid::GetBlockSize() const
{
    return m_BlockSize;
}

std::ostream& operator<<(std::ostream& os, Grid const& grid)
{
    for(auto row : boost::irange(0, grid.GetGridSize()))
    {
        if (row % grid.GetBlockSize() == 0)
            PrintHorizontalLine(os, grid.GetGridSize(), grid.GetBlockSize());

        for(auto col : boost::irange(0, grid.GetGridSize()))
        {
            if (col % grid.GetBlockSize() == 0)
                PrintVerticalSeparator(os);

            PrintCell(os, grid.GetCell(Position{row, col}));
        }

        PrintVerticalSeparator(os);
        os << std::endl;
    }

    PrintHorizontalLine(os, grid.GetGridSize(), grid.GetBlockSize());

    return os;
}

bool operator==(const Grid& lhs, const Grid& rhs)
{
    return lhs.GetGridSize() == rhs.GetGridSize()
            && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

} // namespace sudoku
