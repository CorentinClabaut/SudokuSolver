#include "ParallelPossibilitiesRemover.hpp"

#include <thread>
#include <optional>

#include <boost/range/algorithm.hpp>

#include "PossibilitiesRemover.hpp"
#include "FoundCells.hpp"
#include "Grid.hpp"

using namespace sudoku;

ParallelPossibilitiesRemoverImpl::ParallelPossibilitiesRemoverImpl(
        int parallelThreadsCount,
        std::unique_ptr<PossibilitiesRemover> possibilitiesRemover) :
    m_ParallelThreadsCount(parallelThreadsCount),
    m_PossibilitiesRemover(std::move(possibilitiesRemover))
{}

void ParallelPossibilitiesRemoverImpl::UpdateGrid(FoundCells& foundCells, Grid& grid) const
{
    int threadsWorkingCount = 0;

    std::vector<std::thread> threads(m_ParallelThreadsCount);
    for (auto& thread : threads)
        thread = std::thread([&]{ RemoveQueuedUnvalidPossibilities(foundCells, grid, threadsWorkingCount); });

    boost::for_each(threads, [](auto& t){ t.join(); });
}

void ParallelPossibilitiesRemoverImpl::RemoveQueuedUnvalidPossibilities(FoundCells& foundCells, Grid& grid, int& threadsWorkingCount) const
{
    while (true)
    {
        std::optional<SharedCell> newFoundCell;

        {
        std::lock_guard<std::mutex> l(foundCells.m_Mutex);

        if (!foundCells.m_Queue.empty())
        {
            newFoundCell = foundCells.m_Queue.front();
            foundCells.m_Queue.pop();
            threadsWorkingCount++;
        }
        else if (threadsWorkingCount == 0)
        {
            return;
        }
        }

        if (newFoundCell)
        {
            m_PossibilitiesRemover->UpdateGrid(**newFoundCell, grid, foundCells);

            {
            std::lock_guard<std::mutex> l(foundCells.m_Mutex);
            threadsWorkingCount--;
            }
        }
    }
}
