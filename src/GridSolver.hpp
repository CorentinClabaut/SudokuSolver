#pragma once

#include <memory>

namespace sudoku
{

class ParallelPossibilitiesRemover;
class ParallelUniquePossibilitySetter;
class GridStatusGetter;
struct FoundCells;
struct Grid;
enum class GridStatus;

class GridSolver
{
public:
    virtual ~GridSolver() = default;

    virtual GridStatus Solve(Grid& grid) const = 0;
};

class GridSolverImpl : public GridSolver
{
public:
    GridSolverImpl(
            std::unique_ptr<ParallelPossibilitiesRemover> parallelPossibilitiesRemover,
            std::unique_ptr<ParallelUniquePossibilitySetter> parallelUniquePossibilitySetter,
            std::unique_ptr<GridStatusGetter> gridStatusGetter);

    GridStatus Solve(Grid& grid) const override;

private:
    void GetFoundCells(Grid const& grid, FoundCells& foundCells) const;

    std::unique_ptr<ParallelPossibilitiesRemover> m_ParallelPossibilitiesRemover;
    std::unique_ptr<ParallelUniquePossibilitySetter> m_ParallelUniquePossibilitySetter;
    std::unique_ptr<GridStatusGetter> m_GridStatusGetter;
};

} /* namespace sudoku */

