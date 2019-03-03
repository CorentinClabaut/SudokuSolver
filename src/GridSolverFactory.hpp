#pragma once

#include "GridSolver.hpp"

namespace sudoku
{

class GridSolverFactory
{
public:
    static std::unique_ptr<GridSolver> Make(int parallelThreadsCount);
};

} /* namespace sudoku */

