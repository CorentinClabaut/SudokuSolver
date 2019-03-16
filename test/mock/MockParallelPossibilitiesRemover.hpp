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
    MOCK_CONST_METHOD2(UpdateGrid, bool(FoundCells& foundCells, Grid& grid));
};

} /* namespace test */
} /* namespace sudoku */
