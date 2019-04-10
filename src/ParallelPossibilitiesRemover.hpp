#pragma once

#include <memory>

#include "ThreadPool.hpp"

namespace sudoku
{

class PossibilitiesRemover;
class FoundPositions;
class Grid;

class ParallelPossibilitiesRemover
{
public:
    virtual ~ParallelPossibilitiesRemover() = default;

    virtual void UpdateGrid(FoundPositions& foundPositions, Grid& grid) = 0;
};

class ParallelPossibilitiesRemoverImpl : public ParallelPossibilitiesRemover
{
public:
    ParallelPossibilitiesRemoverImpl(
            int parallelThreadsCount,
            std::shared_ptr<ThreadPool> threadPool,
            std::unique_ptr<PossibilitiesRemover> possibilitiesRemover);

    void UpdateGrid(FoundPositions& foundPositions, Grid& grid) override;

private:
    void RemoveQueuedUnvalidPossibilities(FoundPositions& foundPositions, Grid& grid, int& threadsWorkingCount, std::atomic<bool>& exception) const;

    std::shared_ptr<ThreadPool> m_ThreadPool;

    const int m_ParallelThreadsCount;

    std::unique_ptr<PossibilitiesRemover> m_PossibilitiesRemover;
};

} /* namespace sudoku */

