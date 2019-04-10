#pragma once

#include <memory>

#include "ThreadPool.hpp"

namespace sudoku
{

class Grid;
class FoundPositions;
class UniquePossibilitySetter;

class ParallelUniquePossibilitySetter
{
public:
    virtual ~ParallelUniquePossibilitySetter() = default;

    virtual void SetCellsWithUniquePossibility(Grid& grid, FoundPositions& foundPositions) = 0;
};

class ParallelUniquePossibilitySetterImpl : public ParallelUniquePossibilitySetter
{
public:
    ParallelUniquePossibilitySetterImpl(
            int parallelThreadsCount,
            std::shared_ptr<ThreadPool> threadPool,
            std::unique_ptr<UniquePossibilitySetter> uniquePossibilitySetter);

    void SetCellsWithUniquePossibility(Grid& grid, FoundPositions& foundPositions) override;

private:
    std::shared_ptr<ThreadPool> m_ThreadPool;

    const int m_ParallelThreadsCount;

    std::unique_ptr<UniquePossibilitySetter> m_UniquePossibilitySetter;
};

} /* namespace sudoku */

