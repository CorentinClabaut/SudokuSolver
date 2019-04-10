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
    MOCK_CONST_METHOD2(GetRelatedHorizontalPositions, std::vector<Position>(Position const& selectedPosition, int gridSize));
    MOCK_CONST_METHOD2(GetRelatedVerticalPositions, std::vector<Position>(Position const& selectedPosition, int gridSize));
    MOCK_CONST_METHOD3(GetRelatedBlockPositions, std::vector<Position>(Position const& selectedPosition, int gridSize, int blockSize));
    MOCK_CONST_METHOD3(GetAllRelatedPositions, std::vector<Position>(Position const& selectedPosition, int gridSize, int blockSize));
};

} /* namespace test */
} /* namespace sudoku */
