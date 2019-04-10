#include "ParallelPossibilitiesRemover.hpp"

#include <future>
#include <optional>

#include <boost/range/algorithm.hpp>

#include "PossibilitiesRemover.hpp"
#include "FoundPositions.hpp"
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

void ParallelPossibilitiesRemoverImpl::UpdateGrid(FoundPositions& foundPositions, Grid& grid)
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
            RemoveQueuedUnvalidPossibilities(foundPositions, grid, threadsWorkingCount, exceptionThrown);

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

void ParallelPossibilitiesRemoverImpl::RemoveQueuedUnvalidPossibilities(FoundPositions& foundPositions, Grid& grid, int& threadsWorkingCount, std::atomic<bool>& exception) const
{
    while (!exception)
    {
        std::optional<Position> newFoundPosition;

        {
        std::lock_guard<std::mutex> l(foundPositions.m_Mutex);

        if (!foundPositions.m_Queue.empty())
        {
            newFoundPosition = foundPositions.m_Queue.front();
            foundPositions.m_Queue.pop();
            threadsWorkingCount++;
        }
        else if (threadsWorkingCount == 0)
        {
            return;
        }
        }

        if (newFoundPosition)
        {
            try
            {
                m_PossibilitiesRemover->UpdateGrid(*newFoundPosition, grid, foundPositions);
            }
            catch(std::exception const&)
            {
                exception = true;
            }

            {
            std::lock_guard<std::mutex> l(foundPositions.m_Mutex);
            threadsWorkingCount--;
            }
        }
    }
}
