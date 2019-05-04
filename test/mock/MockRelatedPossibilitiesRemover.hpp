#pragma once

#include "RelatedPossibilitiesRemover.hpp"
#include <gmock/gmock.h>

namespace sudoku
{
namespace test
{

class MockRelatedPossibilitiesRemover : public RelatedPossibilitiesRemover
{
public:
    MOCK_CONST_METHOD3(UpdateRelatedPossibilities, void(Position const& newFoundPosition, Grid& grid, FoundPositions& foundPositions));
};

} /* namespace test */
} /* namespace sudoku */
