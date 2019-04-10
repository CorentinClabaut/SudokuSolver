#pragma once

#include "ParallelPossibilitiesRemover.hpp"
#include <gmock/gmock.h>

namespace sudoku
{
namespace test
{

class MockParallelPossibilitiesRemover : public ParallelPossibilitiesRemover
{
public:
    MOCK_METHOD2(UpdateGrid, void(FoundPositions& foundPositions, Grid& grid));
};

} /* namespace test */
} /* namespace sudoku */
