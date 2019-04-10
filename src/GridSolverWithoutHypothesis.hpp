#pragma once

#include <memory>

namespace sudoku
{

class ParallelPossibilitiesRemover;
class ParallelUniquePossibilitySetter;
struct FoundPositions;
struct Grid;
enum class GridStatus;

class GridSolverWithoutHypothesis
{
public:
    virtual ~GridSolverWithoutHypothesis() = default;

    virtual GridStatus Solve(Grid& grid, FoundPositions& foundPositions) const = 0;
};

class GridSolverWithoutHypothesisImpl : public GridSolverWithoutHypothesis
{
public:
    GridSolverWithoutHypothesisImpl(
            std::unique_ptr<ParallelPossibilitiesRemover> parallelPossibilitiesRemover,
            std::unique_ptr<ParallelUniquePossibilitySetter> parallelUniquePossibilitySetter);

    GridStatus Solve(Grid& grid, FoundPositions& foundPositions) const override;

private:
    std::unique_ptr<ParallelPossibilitiesRemover> m_ParallelPossibilitiesRemover;
    std::unique_ptr<ParallelUniquePossibilitySetter> m_ParallelUniquePossibilitySetter;
};

} /* namespace sudoku */

