#pragma once

#include "RelatedCellsGetter.hpp"
#include <gmock/gmock.h>
#include <boost/range/algorithm/transform.hpp>

#include "Grid.hpp"

namespace sudoku
{
namespace test
{

using testing::Ref;
using testing::Return;

class MockRelatedCellsGetter : public RelatedCellsGetter
{
public:
    MOCK_CONST_METHOD2(GetRelatedHorizontalCells, CellsGroup(Position const& selectedPosition, Grid const& grid));
    MOCK_CONST_METHOD2(GetRelatedVerticalCells, CellsGroup(Position const& selectedPosition, Grid const& grid));
    MOCK_CONST_METHOD2(GetRelatedBlockCells, CellsGroup(Position const& selectedPosition, Grid const& grid));
    MOCK_CONST_METHOD2(GetAllRelatedCells, CellsGroup(Position const& selectedPosition, Grid const& grid));

    void ExpectGetRelatedHorizontalCells(Grid& grid, Position const& selectedPosition, std::vector<Position> const& relatedPositions)
    {
        EXPECT_CALL(*this, GetRelatedHorizontalCells(selectedPosition, Ref(grid))).WillRepeatedly(Return(GetCells(grid, relatedPositions)));
    }

    void ExpectGetRelatedVerticalCells(Grid& grid, Position const& selectedPosition, std::vector<Position> const& relatedPositions)
    {
        EXPECT_CALL(*this, GetRelatedVerticalCells(selectedPosition, Ref(grid))).WillRepeatedly(Return(GetCells(grid, relatedPositions)));
    }

    void ExpectGetRelatedBlockCells(Grid& grid, Position const& selectedPosition, std::vector<Position> const& relatedPositions)
    {
        EXPECT_CALL(*this, GetRelatedBlockCells(selectedPosition, Ref(grid))).WillRepeatedly(Return(GetCells(grid, relatedPositions)));
    }

    void ExpectGetAllRelatedCells(Grid& grid, Position const& selectedPosition, std::vector<Position> const& relatedPositions)
    {
        EXPECT_CALL(*this, GetAllRelatedCells(selectedPosition, Ref(grid))).WillRepeatedly(Return(GetCells(grid, relatedPositions)));
    }

private:
    CellsGroup GetCells(Grid& grid, std::vector<Position> const& relatedPositions)
    {
        CellsGroup cellsGroup;

        boost::transform(relatedPositions, std::back_inserter(cellsGroup), [&](auto const& p){ return grid.m_Cells[p.m_Row][p.m_Col]; });

        return cellsGroup;
    }
};

} /* namespace test */
} /* namespace sudoku */
