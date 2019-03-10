#include "ParallelUniquePossibilitySetter.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "mock/MockUniquePossibilitySetter.hpp"

#include "Grid.hpp"
#include "FoundCells.hpp"
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
                    std::move(m_UniquePossibilitySetter));
    }

    std::unique_ptr<MockUniquePossibilitySetter> m_UniquePossibilitySetter = std::make_unique<StrictMock<MockUniquePossibilitySetter>>();
};

TEST_F(TestParallelUniquePossibilitySetter, SetUniquePossibilitiesInParallel)
{
    const int parallelThreadsCount = 3;
    const int gridSize = 4;

    Grid grid {gridSize};
    FoundCells foundCells;

    ParallelThreadsCounter parallelThreadsCounter;

    auto GetThreadsWorkingInParallel = [&](std::vector<SharedCell>&, Grid const&, FoundCells&) { parallelThreadsCounter.count(); };

    std::vector<SharedCell> thread1Cells = {grid.m_Cells[0][0], grid.m_Cells[0][1], grid.m_Cells[0][2], grid.m_Cells[0][3], grid.m_Cells[1][0], grid.m_Cells[1][1]};
    EXPECT_CALL(*m_UniquePossibilitySetter, SetCellsWithUniquePossibility(thread1Cells, Ref(grid), Ref(foundCells)))
            .WillOnce(Invoke(GetThreadsWorkingInParallel));

    std::vector<SharedCell> thread2Cells = {grid.m_Cells[1][2], grid.m_Cells[1][3], grid.m_Cells[2][0], grid.m_Cells[2][1], grid.m_Cells[2][2], grid.m_Cells[2][3]};
    EXPECT_CALL(*m_UniquePossibilitySetter, SetCellsWithUniquePossibility(thread2Cells, Ref(grid), Ref(foundCells)))
            .WillOnce(Invoke(GetThreadsWorkingInParallel));

    std::vector<SharedCell> thread4Cells = {grid.m_Cells[3][0], grid.m_Cells[3][1], grid.m_Cells[3][2], grid.m_Cells[3][3]};
    EXPECT_CALL(*m_UniquePossibilitySetter, SetCellsWithUniquePossibility(thread4Cells, Ref(grid), Ref(foundCells)))
            .WillOnce(Invoke(GetThreadsWorkingInParallel));

    MakeParallelUniquePossibilitySetter(parallelThreadsCount)->SetCellsWithUniquePossibility(grid, foundCells);

    EXPECT_THAT(parallelThreadsCounter.GetMaxThreadsWorkingInParallel(), Eq(parallelThreadsCount));
}

TEST_F(TestParallelUniquePossibilitySetter, PropagateThreadExceptions)
{
    const int parallelThreadsCount = 3;
    const int gridSize = 4;

    Grid grid {gridSize};
    FoundCells foundCells;

    std::vector<SharedCell> thread1Cells = {grid.m_Cells[0][0], grid.m_Cells[0][1], grid.m_Cells[0][2], grid.m_Cells[0][3], grid.m_Cells[1][0], grid.m_Cells[1][1]};
    EXPECT_CALL(*m_UniquePossibilitySetter, SetCellsWithUniquePossibility(thread1Cells, Ref(grid), Ref(foundCells)))
            .WillOnce(Invoke([&](std::vector<SharedCell>&, Grid const&, FoundCells&)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds((rand() % 10) + 1));
                    throw std::runtime_error("error");
                }));

    std::vector<SharedCell> thread2Cells = {grid.m_Cells[1][2], grid.m_Cells[1][3], grid.m_Cells[2][0], grid.m_Cells[2][1], grid.m_Cells[2][2], grid.m_Cells[2][3]};
    EXPECT_CALL(*m_UniquePossibilitySetter, SetCellsWithUniquePossibility(thread2Cells, Ref(grid), Ref(foundCells)))
            .WillOnce(Invoke([&](std::vector<SharedCell>&, Grid const&, FoundCells&)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds((rand() % 10) + 1));
                }));

    std::vector<SharedCell> thread4Cells = {grid.m_Cells[3][0], grid.m_Cells[3][1], grid.m_Cells[3][2], grid.m_Cells[3][3]};
    EXPECT_CALL(*m_UniquePossibilitySetter, SetCellsWithUniquePossibility(thread4Cells, Ref(grid), Ref(foundCells)))
            .WillOnce(Invoke([&](std::vector<SharedCell>&, Grid const&, FoundCells&)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds((rand() % 10) + 1));
                    throw std::runtime_error("error");
                }));

    try
    {
        MakeParallelUniquePossibilitySetter(parallelThreadsCount)->SetCellsWithUniquePossibility(grid, foundCells);

        FAIL() << "Error: did not propagate the exception";
    }
    catch(std::exception const&)
    {
        SUCCEED() << "Exception thrown in thread handled safely";
    }
}

} /* namespace test */
} /* namespace sudoku */
