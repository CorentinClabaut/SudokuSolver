#pragma once

#include "GridSolverWithoutHypothesis.hpp"
#include <gmock/gmock.h>

namespace sudoku
{
namespace test
{

class MockGridSolverWithoutHypothesis : public GridSolverWithoutHypothesis
{
public:
    MOCK_CONST_METHOD2(Solve, GridStatus(Grid& grid, FoundPositions& foundPositions));
};

} /* namespace test */
} /* namespace sudoku */
