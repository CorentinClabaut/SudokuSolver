#include "ParallelUniquePossibilitySetter.hpp"

#include <future>

#include <condition_variable>
#include <mutex>
#include <atomic>

#include <boost/range/irange.hpp>
#include <boost/range/algorithm.hpp>

#include "Grid.hpp"
#include "FoundPositions.hpp"
#include "UniquePossibilitySetter.hpp"

using namespace sudoku;

class PositionIter
{
public:
    PositionIter(int gridSize) :
        m_GridSize(gridSize),
        m_Pos {0, 0}
    {}

    std::optional<Position> GetAndIncrement()
    {
        std::lock_guard l(m_Mutex);

        if (IsEnd())
            return {};

        auto pos = m_Pos;

        Increment();

        return pos;
    }

private:
    bool IsEnd()
    {
        return m_Pos.m_Row == m_GridSize
                && m_Pos.m_Col == 0;
    }

    void Increment()
    {
        m_Pos.m_Col++;
        m_Pos.m_Col %= m_GridSize;

        if (m_Pos.m_Col == 0)
            m_Pos.m_Row++;
    }

    std::mutex m_Mutex;

    const int m_GridSize;
    Position m_Pos;
};

ParallelUniquePossibilitySetterImpl::ParallelUniquePossibilitySetterImpl(
        int parallelThreadsCount,
        std::shared_ptr<ThreadPool> threadPool,
        std::unique_ptr<UniquePossibilitySetter> uniquePossibilitySetter) :
    m_ThreadPool(threadPool),
    m_ParallelThreadsCount(parallelThreadsCount),
    m_UniquePossibilitySetter(std::move(uniquePossibilitySetter))
{}

void ParallelUniquePossibilitySetterImpl::SetCellsWithUniquePossibility(Grid& grid, FoundPositions& foundPositions)
{
    std::atomic<int> threadsFinished {0};
    std::atomic<bool> exceptionThrown {false};

    std::mutex m;
    auto cv = std::make_shared<std::condition_variable>();

    PositionIter positionIter {grid.GetGridSize()};

    for (int threadId : boost::irange(0, m_ParallelThreadsCount))
    {
        auto threadWork = [&, threadId]
        {
            auto position = positionIter.GetAndIncrement();

            while (position.has_value() && !exceptionThrown)
            {
                try
                {
                    m_UniquePossibilitySetter->SetIfUniquePossibility(*position, grid, foundPositions);
                }
                catch (std::exception const&)
                {
                    exceptionThrown = true;
                }

                position = positionIter.GetAndIncrement();
            }

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
        throw std::runtime_error("exception");
}
