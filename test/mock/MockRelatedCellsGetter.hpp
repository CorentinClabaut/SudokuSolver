#pragma once

#include "RelatedCellsGetter.hpp"
#include <gmock/gmock.h>

namespace sudoku
{
namespace test
{

class MockRelatedCellsGetter : public RelatedCellsGetter
{
public:
    MOCK_CONST_METHOD2(GetRelatedHorizontalCells, CellsGroup(Position const& selectedPosition, Grid const& grid));
    MOCK_CONST_METHOD2(GetRelatedVerticalCells, CellsGroup(Position const& selectedPosition, Grid const& grid));
    MOCK_CONST_METHOD2(GetRelatedBlockCells, CellsGroup(Position const& selectedPosition, Grid const& grid));
};

} /* namespace test */
} /* namespace sudoku */
