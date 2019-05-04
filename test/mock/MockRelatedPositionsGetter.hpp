#pragma once

#include "RelatedPositionsGetter.hpp"
#include <gmock/gmock.h>

namespace sudoku
{
namespace test
{

class MockRelatedPositionsGetter : public RelatedPositionsGetter
{
public:
    MOCK_CONST_METHOD2(GetRelatedHorizontalPositions, Range<Position>(Position const& selectedPosition, int gridSize));
    MOCK_CONST_METHOD2(GetRelatedVerticalPositions, Range<Position>(Position const& selectedPosition, int gridSize));
    MOCK_CONST_METHOD2(GetRelatedBlockPositions, Range<Position>(Position const& selectedPosition, int gridSize));
    MOCK_CONST_METHOD2(GetAllRelatedPositions, Range<Position>(Position const& selectedPosition, int gridSize));
};

} /* namespace test */
} /* namespace sudoku */
