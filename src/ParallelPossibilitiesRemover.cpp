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
        std::shared_ptr<ThreadPool> threadPool,
        std::unique_ptr<PossibilitiesRemover> possibilitiesRemover) :
    m_ThreadPool(threadPool),
    m_ParallelThreadsCount(parallelThreadsCount),
    m_PossibilitiesRemover(std::move(possibilitiesRemover))
{}

void ParallelPossibilitiesRemoverImpl::UpdateGrid(FoundCells& foundCells, Grid& grid)
{
    int threadsWorkingCount = 0;

    std::atomic<int> threadsFinished {0};
    std::atomic<bool> exceptionThrown {false};

    std::mutex m;
    auto cv = std::make_shared<std::condition_variable>();

    for (int threadId = 0; threadId < m_ParallelThreadsCount; threadId++)
    {
        auto threadWork = [&, threadId]
        {
            RemoveQueuedUnvalidPossibilities(foundCells, grid, threadsWorkingCount, exceptionThrown);

            std::unique_lock<std::mutex> lk(m);
            threadsFinished++;
            if (threadsFinished == m_ParallelThreadsCount)
                cv->notify_one();
        };

        m_ThreadPool->Enqueue(threadWork);
    }

    std::unique_lock<std::mutex> lk(m);
    cv->wait(lk, [this, &threadsFinished]{ return threadsFinished == m_ParallelThreadsCount; });

    if (exceptionThrown)
        throw std::runtime_error("UpdateGrid Exception");
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
            catch(std::exception const&)
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
