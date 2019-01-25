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

std::vector<int> CreateIncrementingVector(int firstValue, int length)
{
    std::vector<int> vec(length);

    boost::iota(vec, firstValue);

    return vec;
}

std::vector<Position> GetAllPositions(std::vector<int> const& rows, std::vector<int> const& cols)
{
    std::vector<Position> positions;

    for(auto row : rows)
    {
        for(auto col : cols)
        {
            positions.push_back({row, col});
        }
    }

    return positions;
}

std::vector<Position> GetAllPositionsButSelectedOne(std::vector<int> const& rows, std::vector<int> const& cols, Position const& selectedPosition)
{
    auto positions = GetAllPositions(rows, cols);

    boost::remove_erase(positions, selectedPosition);

    return positions;
}

CellsGroup GetCells(std::vector<Position> const& positions, Grid const& grid)
{
    auto GetCell = [&](auto const& position){ return grid.m_Cells[position.m_Row][position.m_Col]; };

    return boost::copy_range<CellsGroup>( positions | boost::adaptors::transformed(GetCell) );
}

std::vector<Position> GetRelatedHorizontalPositions(Position const& selectedPosition, int gridSize)
{
    std::vector<int> rows {selectedPosition.m_Row};
    std::vector<int> cols = CreateIncrementingVector(0, gridSize);

    return GetAllPositionsButSelectedOne(rows, cols, selectedPosition);
}

std::vector<Position> GetRelatedVerticalPositions(Position const& selectedPosition, int gridSize)
{
    std::vector<int> rows = CreateIncrementingVector(0, gridSize);
    std::vector<int> cols {selectedPosition.m_Col};

    return GetAllPositionsButSelectedOne(rows, cols, selectedPosition);
}

std::vector<Position> GetRelatedBlockPositions(Position const& selectedPosition, int blockSize)
{
    auto beginRow = RoundDown(selectedPosition.m_Row, blockSize);
    auto rows = CreateIncrementingVector(beginRow, blockSize);

    auto beginCol = RoundDown(selectedPosition.m_Col, blockSize);
    std::vector<int> cols = CreateIncrementingVector(beginCol, blockSize);

    return GetAllPositionsButSelectedOne(rows, cols, selectedPosition);
}

std::vector<Position> GetAllRelatedPositions(Position const& selectedPosition, int gridSize, int blockSize)
{
    std::vector<Position> positions;

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
