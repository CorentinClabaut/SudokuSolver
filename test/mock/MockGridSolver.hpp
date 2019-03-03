#pragma once

#include "GridSolver.hpp"
#include <gmock/gmock.h>

namespace sudoku
{
namespace test
{

class MockGridSolver : public GridSolver
{
public:
    MOCK_CONST_METHOD1(Solve, GridStatus(Grid& grid));
};

} /* namespace test */
} /* namespace sudoku */
