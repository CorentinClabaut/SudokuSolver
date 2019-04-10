#pragma once

#include <memory>

namespace sudoku
{

class GridSolverWithoutHypothesis;
struct FoundPositions;
struct Grid;

class GridSolver
{
public:
    virtual ~GridSolver() = default;

    virtual bool Solve(Grid& grid) const = 0;
};

class GridSolverWithHypothesisImpl : public GridSolver
{
public:
    GridSolverWithHypothesisImpl(
            std::unique_ptr<GridSolverWithoutHypothesis> gridSolverWithoutHypothesis);

    bool Solve(Grid& grid) const override;

private:
    bool SolveWithtHypothesis(Grid& grid, FoundPositions& foundPositions) const;

    std::unique_ptr<GridSolverWithoutHypothesis> m_GridSolverWithoutHypothesis;
};

} /* namespace sudoku */

