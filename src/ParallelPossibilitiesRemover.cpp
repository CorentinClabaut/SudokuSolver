#include "ParallelPossibilitiesRemover.hpp"

#include <future>
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

    std::atomic<bool> exception {false};
    std::vector<std::future<void>> futures(m_ParallelThreadsCount);
    for (auto& future : futures)
        future = std::async([&]{ RemoveQueuedUnvalidPossibilities(foundCells, grid, threadsWorkingCount, exception); });

    boost::for_each(futures, [](auto& f){ f.get(); });

    if (exception)
        throw std::runtime_error("Thread throw exception while removing unvalid possibilities");
}

void ParallelPossibilitiesRemoverImpl::RemoveQueuedUnvalidPossibilities(FoundCells& foundCells, Grid& grid, int& threadsWorkingCount, std::atomic<bool>& exception) const
{
    while (!exception)
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
            try
            {
                m_PossibilitiesRemover->UpdateGrid(**newFoundCell, grid, foundCells);
            }
            catch(std::exception const& e)
            {
                exception = true;
            }

            {
            std::lock_guard<std::mutex> l(foundCells.m_Mutex);
            threadsWorkingCount--;
            }
        }
    }
}
