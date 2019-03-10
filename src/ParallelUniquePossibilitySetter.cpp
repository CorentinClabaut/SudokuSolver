#include "ParallelUniquePossibilitySetter.hpp"

#include <future>

#include <boost/range/irange.hpp>
#include <boost/range/algorithm.hpp>

#include "Grid.hpp"
#include "FoundCells.hpp"
#include "UniquePossibilitySetter.hpp"

using namespace sudoku;

namespace
{
int RoundUpDiv(int val, int fact)
{
    int div = val / fact;

    return val % fact> 0 ? ++div : div;
}

std::vector<SharedCell> GetThreadCells(Grid& grid, int threadCellsCount, int threadId)
{
    std::vector<SharedCell> threadCells;

    auto beg = grid.m_Cells.origin() + threadId * threadCellsCount;
    auto end = std::min(beg + threadCellsCount, grid.end());

    threadCells.insert(threadCells.begin(), beg, end);

    return threadCells;
}

std::vector<std::vector<SharedCell>> GetThreadsCells(Grid& grid, int parallelThreadCount)
{
    std::vector<std::vector<SharedCell>> threadsCells(parallelThreadCount);

    const int threadCellsCount = RoundUpDiv(grid.m_Cells.num_elements(), parallelThreadCount);

    for (int threadId : boost::irange(0, parallelThreadCount))
    {
        threadsCells[threadId] = GetThreadCells(grid, threadCellsCount, threadId);
    }

    return threadsCells;
}
} // anonymous namespace

ParallelUniquePossibilitySetterImpl::ParallelUniquePossibilitySetterImpl(
        int parallelThreadsCount,
        std::unique_ptr<UniquePossibilitySetter> uniquePossibilitySetter) :
    m_ParallelThreadsCount(parallelThreadsCount),
    m_UniquePossibilitySetter(std::move(uniquePossibilitySetter))
{}

void ParallelUniquePossibilitySetterImpl::SetCellsWithUniquePossibility(Grid& grid, FoundCells& foundCells) const
{
    auto threadsCells = GetThreadsCells(grid, m_ParallelThreadsCount);

    std::vector<std::future<void>> futures(m_ParallelThreadsCount);

    for (int threadId : boost::irange(0, m_ParallelThreadsCount))
        futures[threadId] = std::async([&, threadId]{ m_UniquePossibilitySetter->SetCellsWithUniquePossibility(threadsCells[threadId], grid, foundCells); });

    boost::for_each(futures, [](auto& f){ f.get(); });
}
