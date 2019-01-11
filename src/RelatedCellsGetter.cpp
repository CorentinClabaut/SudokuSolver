#include "RelatedCellsGetter.hpp"

#include "Position.hpp"
#include "Grid.hpp"

#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm_ext.hpp>

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

CellsGroup GetCells(std::vector<Position> const& positions, Grid const& grid)
{
    auto GetCell = [&](auto const& position){ return grid.m_Cells[position.m_Row][position.m_Col]; };

    return boost::copy_range<CellsGroup>( positions | boost::adaptors::transformed(GetCell) );
}

CellsGroup GetRelatedCells(std::vector<int> const& rows, std::vector<int> const& cols, Position selectedPosition, Grid const& grid)
{
    auto positions = GetAllPositions(rows, cols);

    boost::remove_erase(positions, selectedPosition);

    return GetCells(positions, grid);
}
} // anonymous namespace

CellsGroup RelatedCellsGetterImpl::GetRelatedHorizontalCells(Position const& selectedPosition, Grid const& grid) const
{
    std::vector<int> rows {selectedPosition.m_Row};
    std::vector<int> cols = CreateIncrementingVector(0, grid.m_GridSize);

    return GetRelatedCells(rows, cols, selectedPosition, grid);
}

CellsGroup RelatedCellsGetterImpl::GetRelatedVerticalCells(Position const& selectedPosition, Grid const& grid) const
{
    std::vector<int> rows = CreateIncrementingVector(0, grid.m_GridSize);
    std::vector<int> cols {selectedPosition.m_Col};

    return GetRelatedCells(rows, cols, selectedPosition, grid);
}

CellsGroup RelatedCellsGetterImpl::GetRelatedBlockCells(Position const& selectedPosition, Grid const& grid) const
{
    auto beginRow = RoundDown(selectedPosition.m_Row, grid.m_BlockSize);
    auto rows = CreateIncrementingVector(beginRow, grid.m_BlockSize);

    auto beginCol = RoundDown(selectedPosition.m_Col, grid.m_BlockSize);
    std::vector<int> cols = CreateIncrementingVector(beginCol, grid.m_BlockSize);

    return GetRelatedCells(rows, cols, selectedPosition, grid);
}
