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
    MOCK_CONST_METHOD2(SetCellsWithUniquePossibility, void(Grid& grid, FoundCells& foundCells));
};

} /* namespace test */
} /* namespace sudoku */
