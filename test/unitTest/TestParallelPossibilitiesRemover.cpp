#include "ParallelPossibilitiesRemover.hpp"

#include <atomic>
#include <chrono>
#include <thread>

#include <boost/range/irange.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "FoundPositions.hpp"
#include "Grid.hpp"
#include "utils/Utils.hpp"
#include "ThreadPool.hpp"

#include "mock/MockPossibilitiesRemover.hpp"

using testing::_;
using testing::Eq;
using testing::Ref;
using testing::Return;
using testing::Invoke;
using testing::StrictMock;

namespace sudoku
{
namespace test
{

class TestParallelPossibilitiesRemover : public ::testing::Test
{
public:
    TestParallelPossibilitiesRemover()
    {}

    std::unique_ptr<ParallelPossibilitiesRemover> MakeParallelPossibilitiesRemover(int parallelThreadsCount)
    {
        return std::make_unique<ParallelPossibilitiesRemoverImpl>(
                    parallelThreadsCount,
                    std::make_shared<ThreadPool>(parallelThreadsCount),
                    std::move(m_PossibilitiesRemover));
    }

    const int m_GridSize {4};

    std::shared_ptr<FoundPositions> m_FoundPositions = std::make_shared<FoundPositions>();
    Grid m_Grid {m_GridSize};

    ParallelThreadsCounter m_ParallelThreadsCounter;

    std::unique_ptr<MockPossibilitiesRemover> m_PossibilitiesRemover = std::make_unique<StrictMock<MockPossibilitiesRemover>>();
};

TEST_F(TestParallelPossibilitiesRemover, RemoveFoundCellRelatedPossibilities)
{
    m_FoundPositions->m_Queue.push(Position {1, 2});

    EXPECT_CALL(*m_PossibilitiesRemover, UpdateGrid(Position {1, 2}, Ref(m_Grid), Ref(*m_FoundPositions))).Times(1);

    const int parallelThreadsCount {4};
    MakeParallelPossibilitiesRemover(parallelThreadsCount)->UpdateGrid(*m_FoundPositions, m_Grid);

    EXPECT_TRUE(m_FoundPositions->m_Queue.empty());
}

TEST_F(TestParallelPossibilitiesRemover, RemoveSeveralFoundPositionsRelatedPossibilitiesInParallel)
{
    const int parallelThreadsCount {4};

    m_FoundPositions->m_Queue.push(Position {0, 0});
    m_FoundPositions->m_Queue.push(Position {0, 1});
    m_FoundPositions->m_Queue.push(Position {0, 2});
    m_FoundPositions->m_Queue.push(Position {0, 3});

    auto GetThreadsWorkingInParallel = [&](Position const&, Grid&, FoundPositions&) { m_ParallelThreadsCounter.count(); };

    EXPECT_CALL(*m_PossibilitiesRemover, UpdateGrid(Position {0, 0}, Ref(m_Grid), Ref(*m_FoundPositions)))
            .WillOnce(Invoke(GetThreadsWorkingInParallel));

    EXPECT_CALL(*m_PossibilitiesRemover, UpdateGrid(Position {0, 1}, Ref(m_Grid), Ref(*m_FoundPositions)))
            .WillOnce(Invoke(GetThreadsWorkingInParallel));

    EXPECT_CALL(*m_PossibilitiesRemover, UpdateGrid(Position {0, 2}, Ref(m_Grid), Ref(*m_FoundPositions)))
            .WillOnce(Invoke(GetThreadsWorkingInParallel));

    EXPECT_CALL(*m_PossibilitiesRemover, UpdateGrid(Position {0, 3}, Ref(m_Grid), Ref(*m_FoundPositions)))
            .WillOnce(Invoke(GetThreadsWorkingInParallel));

    MakeParallelPossibilitiesRemover(parallelThreadsCount)->UpdateGrid(*m_FoundPositions, m_Grid);

    EXPECT_THAT(m_ParallelThreadsCounter.GetMaxThreadsWorkingInParallel(), Eq(parallelThreadsCount));
    EXPECT_TRUE(m_FoundPositions->m_Queue.empty());
}

TEST_F(TestParallelPossibilitiesRemover, NewCellsFoundByThreadProcessedByOtherThreadsInParallel)
{
    const int testExecutionCount = 20;
    for([[gnu::unused]] int i : boost::irange(0, testExecutionCount))
    {
        const int parallelThreadsCount {4};

        m_FoundPositions->m_Queue.push(Position {1, 1});

        auto FindNewCells = [&](Position const&, Grid& grid, FoundPositions& foundPositions)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));

            foundPositions.Enqueue(grid.GetCell(Position{0, 0}).GetPosition());
            foundPositions.Enqueue(grid.GetCell(Position{0, 1}).GetPosition());
            foundPositions.Enqueue(grid.GetCell(Position{0, 2}).GetPosition());
            foundPositions.Enqueue(grid.GetCell(Position{0, 3}).GetPosition());
        };

        auto GetThreadsWorkingInParallel = [&](Position const&, Grid&, FoundPositions&) { m_ParallelThreadsCounter.count(); };

        m_PossibilitiesRemover = std::make_unique<StrictMock<MockPossibilitiesRemover>>();

        EXPECT_CALL(*m_PossibilitiesRemover, UpdateGrid(Position {1, 1}, Ref(m_Grid), Ref(*m_FoundPositions)))
                .WillOnce(Invoke(FindNewCells));

        EXPECT_CALL(*m_PossibilitiesRemover, UpdateGrid(Position {0, 0}, Ref(m_Grid), Ref(*m_FoundPositions)))
                .WillOnce(Invoke(GetThreadsWorkingInParallel));

        EXPECT_CALL(*m_PossibilitiesRemover, UpdateGrid(Position {0, 1}, Ref(m_Grid), Ref(*m_FoundPositions)))
                .WillOnce(Invoke(GetThreadsWorkingInParallel));

        EXPECT_CALL(*m_PossibilitiesRemover, UpdateGrid(Position {0, 2}, Ref(m_Grid), Ref(*m_FoundPositions)))
                .WillOnce(Invoke(GetThreadsWorkingInParallel));

        EXPECT_CALL(*m_PossibilitiesRemover, UpdateGrid(Position {0, 3}, Ref(m_Grid), Ref(*m_FoundPositions)))
                .WillOnce(Invoke(GetThreadsWorkingInParallel));

        MakeParallelPossibilitiesRemover(parallelThreadsCount)->UpdateGrid(*m_FoundPositions, m_Grid);

        EXPECT_THAT(m_ParallelThreadsCounter.GetMaxThreadsWorkingInParallel(), Eq(parallelThreadsCount));
        EXPECT_TRUE(m_FoundPositions->m_Queue.empty());
    }
}

TEST_F(TestParallelPossibilitiesRemover, AllThreadsStopCorrectlyIfOneThreadStopAfterCatchingAnException)
{
    const int testExecutionCount = 50;
    for([[gnu::unused]] int i : boost::irange(0, testExecutionCount))
    {
        m_PossibilitiesRemover = std::make_unique<StrictMock<MockPossibilitiesRemover>>();

        const int parallelThreadsCount {4};

        m_FoundPositions->m_Queue.push(Position {0, 0});
        m_FoundPositions->m_Queue.push(Position {0, 1});
        m_FoundPositions->m_Queue.push(Position {0, 2});
        m_FoundPositions->m_Queue.push(Position {0, 3});

        EXPECT_CALL(*m_PossibilitiesRemover, UpdateGrid(Position {0, 0}, Ref(m_Grid), Ref(*m_FoundPositions)))
                .WillRepeatedly(Invoke([this](Position const&, Grid&, FoundPositions&)
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds((rand() % 5) + 1));
                    }));

        EXPECT_CALL(*m_PossibilitiesRemover, UpdateGrid(Position {0, 1}, Ref(m_Grid), Ref(*m_FoundPositions)))
                .WillOnce(Invoke([](Position const&, Grid&, FoundPositions&)
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds((rand() % 5) + 1));
                        throw std::runtime_error("error");
                    }));

        EXPECT_CALL(*m_PossibilitiesRemover, UpdateGrid(Position {0, 2}, Ref(m_Grid), Ref(*m_FoundPositions)))
                .WillRepeatedly(Invoke([this](Position const&, Grid&, FoundPositions&)
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds((rand() % 5) + 1));
                    }));

        EXPECT_CALL(*m_PossibilitiesRemover, UpdateGrid(Position {0, 3}, Ref(m_Grid), Ref(*m_FoundPositions)))
                .WillRepeatedly(Invoke([this](Position const&, Grid&, FoundPositions&)
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds((rand() % 5) + 1));
                    }));

        EXPECT_THROW(MakeParallelPossibilitiesRemover(parallelThreadsCount)->UpdateGrid(*m_FoundPositions, m_Grid), std::exception);
    }
}

} /* namespace test */
} /* namespace sudoku */
