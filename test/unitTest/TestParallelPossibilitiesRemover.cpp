#include "ParallelPossibilitiesRemover.hpp"

#include <atomic>
#include <chrono>
#include <thread>

#include <boost/range/irange.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "FoundCells.hpp"
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

    std::shared_ptr<FoundCells> m_FoundCells = std::make_shared<FoundCells>();
    Grid m_Grid {m_GridSize};

    ParallelThreadsCounter m_ParallelThreadsCounter;

    std::unique_ptr<MockPossibilitiesRemover> m_PossibilitiesRemover = std::make_unique<StrictMock<MockPossibilitiesRemover>>();
};

TEST_F(TestParallelPossibilitiesRemover, RemoveFoundCellRelatedPossibilities)
{
    m_FoundCells->m_Queue.push(m_Grid.m_Cells[1][2]);

    EXPECT_CALL(*m_PossibilitiesRemover, UpdateGrid(Ref(*m_Grid.m_Cells[1][2]), Ref(m_Grid), Ref(*m_FoundCells))).Times(1);

    const int parallelThreadsCount {4};
    MakeParallelPossibilitiesRemover(parallelThreadsCount)->UpdateGrid(*m_FoundCells, m_Grid);

    EXPECT_TRUE(m_FoundCells->m_Queue.empty());
}

TEST_F(TestParallelPossibilitiesRemover, RemoveSeveralFoundCellsRelatedPossibilitiesInParallel)
{
    const int parallelThreadsCount {4};

    m_FoundCells->m_Queue.push(m_Grid.m_Cells[0][0]);
    m_FoundCells->m_Queue.push(m_Grid.m_Cells[0][1]);
    m_FoundCells->m_Queue.push(m_Grid.m_Cells[0][2]);
    m_FoundCells->m_Queue.push(m_Grid.m_Cells[0][3]);

    auto GetThreadsWorkingInParallel = [&](Cell const&, Grid&, FoundCells&) { m_ParallelThreadsCounter.count(); };

    EXPECT_CALL(*m_PossibilitiesRemover, UpdateGrid(Ref(*m_Grid.m_Cells[0][0]), Ref(m_Grid), Ref(*m_FoundCells)))
            .WillOnce(Invoke(GetThreadsWorkingInParallel));

    EXPECT_CALL(*m_PossibilitiesRemover, UpdateGrid(Ref(*m_Grid.m_Cells[0][1]), Ref(m_Grid), Ref(*m_FoundCells)))
            .WillOnce(Invoke(GetThreadsWorkingInParallel));

    EXPECT_CALL(*m_PossibilitiesRemover, UpdateGrid(Ref(*m_Grid.m_Cells[0][2]), Ref(m_Grid), Ref(*m_FoundCells)))
            .WillOnce(Invoke(GetThreadsWorkingInParallel));

    EXPECT_CALL(*m_PossibilitiesRemover, UpdateGrid(Ref(*m_Grid.m_Cells[0][3]), Ref(m_Grid), Ref(*m_FoundCells)))
            .WillOnce(Invoke(GetThreadsWorkingInParallel));

    MakeParallelPossibilitiesRemover(parallelThreadsCount)->UpdateGrid(*m_FoundCells, m_Grid);

    EXPECT_THAT(m_ParallelThreadsCounter.GetMaxThreadsWorkingInParallel(), Eq(parallelThreadsCount));
    EXPECT_TRUE(m_FoundCells->m_Queue.empty());
}

TEST_F(TestParallelPossibilitiesRemover, NewCellsFoundByThreadProcessedByOtherThreadsInParallel)
{
    const int testExecutionCount = 20;
    for([[gnu::unused]] int i : boost::irange(0, testExecutionCount))
    {
        const int parallelThreadsCount {4};

        m_FoundCells->m_Queue.push(m_Grid.m_Cells[1][1]);

        auto FindNewCells = [&](Cell const&, Grid& grid, FoundCells& foundCells)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));

            foundCells.Enqueue(grid.m_Cells[0][0]);
            foundCells.Enqueue(grid.m_Cells[0][1]);
            foundCells.Enqueue(grid.m_Cells[0][2]);
            foundCells.Enqueue(grid.m_Cells[0][3]);
        };

        auto GetThreadsWorkingInParallel = [&](Cell const&, Grid&, FoundCells&) { m_ParallelThreadsCounter.count(); };

        m_PossibilitiesRemover = std::make_unique<StrictMock<MockPossibilitiesRemover>>();

        EXPECT_CALL(*m_PossibilitiesRemover, UpdateGrid(Ref(*m_Grid.m_Cells[1][1]), Ref(m_Grid), Ref(*m_FoundCells)))
                .WillOnce(Invoke(FindNewCells));

        EXPECT_CALL(*m_PossibilitiesRemover, UpdateGrid(Ref(*m_Grid.m_Cells[0][0]), Ref(m_Grid), Ref(*m_FoundCells)))
                .WillOnce(Invoke(GetThreadsWorkingInParallel));

        EXPECT_CALL(*m_PossibilitiesRemover, UpdateGrid(Ref(*m_Grid.m_Cells[0][1]), Ref(m_Grid), Ref(*m_FoundCells)))
                .WillOnce(Invoke(GetThreadsWorkingInParallel));

        EXPECT_CALL(*m_PossibilitiesRemover, UpdateGrid(Ref(*m_Grid.m_Cells[0][2]), Ref(m_Grid), Ref(*m_FoundCells)))
                .WillOnce(Invoke(GetThreadsWorkingInParallel));

        EXPECT_CALL(*m_PossibilitiesRemover, UpdateGrid(Ref(*m_Grid.m_Cells[0][3]), Ref(m_Grid), Ref(*m_FoundCells)))
                .WillOnce(Invoke(GetThreadsWorkingInParallel));

        MakeParallelPossibilitiesRemover(parallelThreadsCount)->UpdateGrid(*m_FoundCells, m_Grid);

        EXPECT_THAT(m_ParallelThreadsCounter.GetMaxThreadsWorkingInParallel(), Eq(parallelThreadsCount));
        EXPECT_TRUE(m_FoundCells->m_Queue.empty());
    }
}

TEST_F(TestParallelPossibilitiesRemover, AllThreadsStopCorrectlyIfOneThreadStopAfterCatchingAnException)
{
    const int testExecutionCount = 50;
    for([[gnu::unused]] int i : boost::irange(0, testExecutionCount))
    {
        m_PossibilitiesRemover = std::make_unique<StrictMock<MockPossibilitiesRemover>>();

        const int parallelThreadsCount {4};

        m_FoundCells->m_Queue.push(m_Grid.m_Cells[0][0]);
        m_FoundCells->m_Queue.push(m_Grid.m_Cells[0][1]);
        m_FoundCells->m_Queue.push(m_Grid.m_Cells[0][2]);
        m_FoundCells->m_Queue.push(m_Grid.m_Cells[0][3]);

        EXPECT_CALL(*m_PossibilitiesRemover, UpdateGrid(Ref(*m_Grid.m_Cells[0][0]), Ref(m_Grid), Ref(*m_FoundCells)))
                .WillRepeatedly(Invoke([this](Cell const&, Grid&, FoundCells&)
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds((rand() % 5) + 1));
                    }));

        EXPECT_CALL(*m_PossibilitiesRemover, UpdateGrid(Ref(*m_Grid.m_Cells[0][1]), Ref(m_Grid), Ref(*m_FoundCells)))
                .WillOnce(Invoke([](Cell const&, Grid&, FoundCells&)
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds((rand() % 5) + 1));
                        throw std::runtime_error("error");
                    }));

        EXPECT_CALL(*m_PossibilitiesRemover, UpdateGrid(Ref(*m_Grid.m_Cells[0][2]), Ref(m_Grid), Ref(*m_FoundCells)))
                .WillRepeatedly(Invoke([this](Cell const&, Grid&, FoundCells&)
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds((rand() % 5) + 1));
                    }));

        EXPECT_CALL(*m_PossibilitiesRemover, UpdateGrid(Ref(*m_Grid.m_Cells[0][3]), Ref(m_Grid), Ref(*m_FoundCells)))
                .WillRepeatedly(Invoke([this](Cell const&, Grid&, FoundCells&)
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds((rand() % 5) + 1));
                    }));

        EXPECT_THROW(MakeParallelPossibilitiesRemover(parallelThreadsCount)->UpdateGrid(*m_FoundCells, m_Grid), std::exception);
    }
}

} /* namespace test */
} /* namespace sudoku */
