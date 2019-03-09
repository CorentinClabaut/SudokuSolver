#include "RelatedCellsGetter.hpp"

#include "Position.hpp"
#include "Grid.hpp"

#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm_ext.hpp>
#include <boost/range/algorithm.hpp>

using namespace sudoku;

namespace
{
int RoundDown(int value, int multiplier)
{
    return (value / multiplier) * multiplier;
}

std::vector<Position> GetAllPositionsButSelectedOne(int begRow, int rows, int begCol, int cols, Position const& selectedPosition)
{
    std::vector<Position> positions;
    positions.reserve(rows * cols);

    for(auto row : boost::irange(begRow, begRow + rows))
    {
        for(auto col : boost::irange(begCol, begCol + cols))
        {
            Position pos {row, col};

            if (pos == selectedPosition)
                continue;

            positions.push_back(std::move(pos));
        }
    }

    return positions;
}

CellsGroup GetCells(std::vector<Position> const& positions, Grid const& grid)
{
    auto GetCell = [&](auto const& position){ return grid.m_Cells[position.m_Row][position.m_Col]; };

    return boost::copy_range<CellsGroup>( positions | boost::adaptors::transformed(GetCell) );
}

std::vector<Position> GetRelatedHorizontalPositions(Position const& selectedPosition, int gridSize)
{
    return GetAllPositionsButSelectedOne(selectedPosition.m_Row, 1, 0, gridSize, selectedPosition);
}

std::vector<Position> GetRelatedVerticalPositions(Position const& selectedPosition, int gridSize)
{
    return GetAllPositionsButSelectedOne(0, gridSize, selectedPosition.m_Col, 1, selectedPosition);
}

std::vector<Position> GetRelatedBlockPositions(Position const& selectedPosition, int blockSize)
{
    auto beginRow = RoundDown(selectedPosition.m_Row, blockSize);
    auto beginCol = RoundDown(selectedPosition.m_Col, blockSize);

    return GetAllPositionsButSelectedOne(beginRow, blockSize, beginCol, blockSize, selectedPosition);
}

std::vector<Position> GetAllRelatedPositions(Position const& selectedPosition, int gridSize, int blockSize)
{
    std::vector<Position> positions;
    positions.reserve(gridSize * 3);

    boost::range::push_back(positions, GetRelatedHorizontalPositions(selectedPosition, gridSize));
    boost::range::push_back(positions, GetRelatedVerticalPositions(selectedPosition, gridSize));
    boost::range::push_back(positions, GetRelatedBlockPositions(selectedPosition, blockSize));

    boost::sort(positions);

    return boost::copy_range<std::vector<Position>>( boost::range::unique(positions) );
}
} // anonymous namespace

CellsGroup RelatedCellsGetterImpl::GetRelatedHorizontalCells(Position const& selectedPosition, Grid const& grid) const
{
    auto positions = GetRelatedHorizontalPositions(selectedPosition, grid.m_GridSize);

    return GetCells(positions, grid);
}

CellsGroup RelatedCellsGetterImpl::GetRelatedVerticalCells(Position const& selectedPosition, Grid const& grid) const
{
    auto positions = GetRelatedVerticalPositions(selectedPosition, grid.m_GridSize);

    return GetCells(positions, grid);
}

CellsGroup RelatedCellsGetterImpl::GetRelatedBlockCells(Position const& selectedPosition, Grid const& grid) const
{
    auto positions = GetRelatedBlockPositions(selectedPosition, grid.m_BlockSize);

    return GetCells(positions, grid);
}

CellsGroup RelatedCellsGetterImpl::GetAllRelatedCells(Position const& selectedPosition, Grid const& grid) const
{
    auto positions = GetAllRelatedPositions(selectedPosition, grid.m_GridSize, grid.m_BlockSize);

    return GetCells(positions, grid);
}
