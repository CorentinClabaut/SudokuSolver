#pragma once

#include "ParallelUniquePossibilitySetter.hpp"
#include <gmock/gmock.h>

namespace sudoku
{
namespace test
{

class MockParallelUniquePossibilitySetter : public ParallelUniquePossibilitySetter
{
public:
    MOCK_METHOD2(SetCellsWithUniquePossibility, void(Grid& grid, FoundPositions& foundPositions));
};

} /* namespace test */
} /* namespace sudoku */
