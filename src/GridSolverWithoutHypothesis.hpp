#pragma once

#include <memory>

#include "FoundPositions.hpp"

namespace sudoku
{

class GridPossibilitiesUpdater;
class UniquePossibilitySetter;
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
            std::unique_ptr<GridPossibilitiesUpdater> gridPossibilitiesUpdater,
            std::unique_ptr<UniquePossibilitySetter> uniquePossibilitySetter);

    GridStatus Solve(Grid& grid, FoundPositions& foundPositions) const override;

private:
    std::unique_ptr<GridPossibilitiesUpdater> m_GridPossibilitiesUpdater;
    std::unique_ptr<UniquePossibilitySetter> m_UniquePossibilitySetter;
};

} /* namespace sudoku */

