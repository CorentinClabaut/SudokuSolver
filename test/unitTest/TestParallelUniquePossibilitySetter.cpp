#include "ParallelUniquePossibilitySetter.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <boost/range/irange.hpp>

#include "mock/MockUniquePossibilitySetter.hpp"

#include "Grid.hpp"
#include "FoundPositions.hpp"
#include "ThreadPool.hpp"
#include "utils/Utils.hpp"

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

class TestParallelUniquePossibilitySetter : public ::testing::Test
{
public:
    TestParallelUniquePossibilitySetter()
    {}

    std::unique_ptr<ParallelUniquePossibilitySetter> MakeParallelUniquePossibilitySetter(int parallelThreadsCount)
    {
        return std::make_unique<ParallelUniquePossibilitySetterImpl>(
                    parallelThreadsCount,
                    std::make_shared<ThreadPool>(parallelThreadsCount),
                    std::move(m_UniquePossibilitySetter));
    }

    std::unique_ptr<MockUniquePossibilitySetter> m_UniquePossibilitySetter = std::make_unique<StrictMock<MockUniquePossibilitySetter>>();
};

TEST_F(TestParallelUniquePossibilitySetter, SetUniquePossibilitiesInParallel)
{
    const int parallelThreadsCount = 3;
    const int gridSize = 4;

    Grid grid {gridSize};
    FoundPositions foundPositions;

    ParallelThreadsCounter parallelThreadsCounter;

    auto GetThreadsWorkingInParallel = [&](Position const&, Grid&, FoundPositions&) { parallelThreadsCounter.count(); };

    for (auto row = 0; row < gridSize; row++)
    {
        for (auto col = 0; col < gridSize; col++)
        {
            EXPECT_CALL(*m_UniquePossibilitySetter, SetIfUniquePossibility(Position {row, col}, Ref(grid), Ref(foundPositions)))
                        .WillOnce(Invoke(GetThreadsWorkingInParallel));
        }
    }

    MakeParallelUniquePossibilitySetter(parallelThreadsCount)->SetCellsWithUniquePossibility(grid, foundPositions);

    EXPECT_THAT(parallelThreadsCounter.GetMaxThreadsWorkingInParallel(), Eq(parallelThreadsCount));
}

TEST_F(TestParallelUniquePossibilitySetter, PropagateThreadExceptions)
{
    const int parallelThreadsCount = 3;
    const int gridSize = 4;

    Grid grid {gridSize};
    FoundPositions foundPositions;

    ParallelThreadsCounter parallelThreadsCounter;
    auto GetThreadsWorkingInParallel = [&](Position const&, Grid&, FoundPositions&) { parallelThreadsCounter.count(); };

    const int testExecutionCount = 20;
    for([[gnu::unused]] int i : boost::irange(0, testExecutionCount))
    {
        m_UniquePossibilitySetter = std::make_unique<StrictMock<MockUniquePossibilitySetter>>();

        Position positionThrowing {(rand() % gridSize), (rand() % gridSize)};

        for (auto row = 0; row < gridSize; row++)
        {
            for (auto col = 0; col < gridSize; col++)
            {
                Position position {row, col};

                if (position == positionThrowing)
                {
                    EXPECT_CALL(*m_UniquePossibilitySetter, SetIfUniquePossibility(position, Ref(grid), Ref(foundPositions)))
                            .WillOnce(Invoke([&](Position const&, Grid&, FoundPositions&)
                                {
                                    throw std::runtime_error("error");
                                }));
                }
                else
                {
                    EXPECT_CALL(*m_UniquePossibilitySetter, SetIfUniquePossibility(position, Ref(grid), Ref(foundPositions)))
                                .WillRepeatedly(Invoke(GetThreadsWorkingInParallel));
                }
            }
        }

        try
        {
            MakeParallelUniquePossibilitySetter(parallelThreadsCount)->SetCellsWithUniquePossibility(grid, foundPositions);

            FAIL() << "Error: did not propagate the exception";
        }
        catch(std::exception const&)
        {
            SUCCEED() << "Exception thrown in thread handled safely";
        }
    }
}

} /* namespace test */
} /* namespace sudoku */
