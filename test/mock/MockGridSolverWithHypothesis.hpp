#pragma once

#include "GridSolverWithHypothesis.hpp"
#include <gmock/gmock.h>

namespace sudoku
{
namespace test
{

class MockGridSolver : public GridSolver
{
public:
    MOCK_CONST_METHOD1(Solve, bool(Grid& grid));
};

} /* namespace test */
} /* namespace sudoku */
