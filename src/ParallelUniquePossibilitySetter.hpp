#pragma once

#include <memory>

namespace sudoku
{

class Grid;
class FoundCells;
class UniquePossibilitySetter;

class ParallelUniquePossibilitySetter
{
public:
    virtual ~ParallelUniquePossibilitySetter() = default;

    virtual void SetCellsWithUniquePossibility(Grid& grid, FoundCells& foundCells) const = 0;
};

class ParallelUniquePossibilitySetterImpl : public ParallelUniquePossibilitySetter
{
public:
    ParallelUniquePossibilitySetterImpl(
            int parallelThreadsCount,
            std::unique_ptr<UniquePossibilitySetter> uniquePossibilitySetter);

    void SetCellsWithUniquePossibility(Grid& grid, FoundCells& foundCells) const override;

private:
    const int m_ParallelThreadsCount;

    std::unique_ptr<UniquePossibilitySetter> m_UniquePossibilitySetter;
};

} /* namespace sudoku */

