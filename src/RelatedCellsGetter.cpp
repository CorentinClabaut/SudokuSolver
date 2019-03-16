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

void GetAllPositionsButSelectedOne(int begRow, int rows, int begCol, int cols, Position const& selectedPosition, std::vector<Position>& positions)
{
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
}

CellsGroup GetCells(std::vector<Position> const& positions, Grid const& grid)
{
    CellsGroup cellsGroup(positions.size());

    std::transform(positions.begin(), positions.end(), cellsGroup.begin(), [&](auto const& position){ return grid.m_Cells[position.m_Row][position.m_Col]; });

    return cellsGroup;
}

void GetRelatedHorizontalPositions(Position const& selectedPosition, int gridSize, std::vector<Position>& positions)
{
    GetAllPositionsButSelectedOne(selectedPosition.m_Row, 1, 0, gridSize, selectedPosition, positions);
}

void GetRelatedVerticalPositions(Position const& selectedPosition, int gridSize, std::vector<Position>& positions)
{
    GetAllPositionsButSelectedOne(0, gridSize, selectedPosition.m_Col, 1, selectedPosition, positions);
}

void GetRelatedBlockPositions(Position const& selectedPosition, int blockSize, std::vector<Position>& positions)
{
    auto beginRow = RoundDown(selectedPosition.m_Row, blockSize);
    auto beginCol = RoundDown(selectedPosition.m_Col, blockSize);

    GetAllPositionsButSelectedOne(beginRow, blockSize, beginCol, blockSize, selectedPosition, positions);
}

void GetAllRelatedPositions(Position const& selectedPosition, int gridSize, int blockSize, std::vector<Position>& positions)
{
    GetRelatedHorizontalPositions(selectedPosition, gridSize, positions);
    GetRelatedVerticalPositions(selectedPosition, gridSize, positions);
    GetRelatedBlockPositions(selectedPosition, blockSize, positions);

    std::sort(positions.begin(), positions.end());
    auto end = std::unique(positions.begin(), positions.end());
    positions.erase(end, positions.end());
}
} // anonymous namespace

CellsGroup RelatedCellsGetterImpl::GetRelatedHorizontalCells(Position const& selectedPosition, Grid const& grid) const
{
    std::vector<Position> positions;
    positions.reserve(grid.m_GridSize);

    GetRelatedHorizontalPositions(selectedPosition, grid.m_GridSize, positions);

    return GetCells(positions, grid);
}

CellsGroup RelatedCellsGetterImpl::GetRelatedVerticalCells(Position const& selectedPosition, Grid const& grid) const
{
    std::vector<Position> positions;
    positions.reserve(grid.m_GridSize);

    GetRelatedVerticalPositions(selectedPosition, grid.m_GridSize, positions);

    return GetCells(positions, grid);
}

CellsGroup RelatedCellsGetterImpl::GetRelatedBlockCells(Position const& selectedPosition, Grid const& grid) const
{
    std::vector<Position> positions;
    positions.reserve(grid.m_GridSize);

    GetRelatedBlockPositions(selectedPosition, grid.m_BlockSize, positions);

    return GetCells(positions, grid);
}

CellsGroup RelatedCellsGetterImpl::GetAllRelatedCells(Position const& selectedPosition, Grid const& grid) const
{
    std::vector<Position> positions;
    positions.reserve(grid.m_GridSize * 3);

    GetAllRelatedPositions(selectedPosition, grid.m_GridSize, grid.m_BlockSize, positions);

    return GetCells(positions, grid);
}
