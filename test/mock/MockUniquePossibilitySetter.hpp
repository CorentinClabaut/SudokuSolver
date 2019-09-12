#pragma once

#include "UniquePossibilitySetter.hpp"
#include <gmock/gmock.h>

namespace sudoku
{
namespace test
{

class MockUniquePossibilitySetter : public UniquePossibilitySetter
{
public:
    MOCK_CONST_METHOD2(SetCellsWithUniquePossibility, void(Grid& grid, FoundPositions& foundPositions));
};

} /* namespace test */
} /* namespace sudoku */
