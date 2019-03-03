#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "GridSolverFactory.hpp"
#include "GridStatus.hpp"
#include "Grid.hpp"
#include "utils/Utils.hpp"

using testing::Eq;
using testing::Ne;

namespace sudoku
{
namespace test
{

class FTestGridSolver : public ::testing::Test
{
public:
    FTestGridSolver()
    {}
};

TEST_F(FTestGridSolver, Solve4x4)
{
    const int gridSize {4};
    const int cellsKept {8};

    const auto positionsValues = CreatePositionsValues4x4();
    const auto expectedGrid = CreateGrid(gridSize, positionsValues);

    const int testExecutionCount = 50;
    for(int testId : boost::irange(1, testExecutionCount + 1))
    {
        const int parallelThreadsCount {testId % 5 + 1};
        auto gridSolver = GridSolverFactory::Make(parallelThreadsCount);

        auto grid = CreateGrid(gridSize, KeepRandomCells(positionsValues, cellsKept));

        try
        {
            const auto gridStatus = gridSolver->Solve(grid);

            //TODO [hypothesis]: check EXPECT_THAT(gridStatus, Eq(GridStatus::SolvedCorrectly));
            EXPECT_THAT(gridStatus, Ne(GridStatus::Wrong));
            if (gridStatus != GridStatus::Incomplete)
            {
                EXPECT_THAT(grid, Eq(expectedGrid));
            }
        }
        catch(std::exception& e)
        {
            FAIL() << "Couldn't solve grid because: " << e.what();
        }
    }
}

TEST_F(FTestGridSolver, Solve9x9)
{
    const int gridSize {9};
    const int cellsKept {40};

    const auto positionsValues = CreatePositionsValues9x9();
    const auto expectedGrid = CreateGrid(gridSize, positionsValues);

    const int testExecutionCount = 50;
    for(int testId : boost::irange(1, testExecutionCount + 1))
    {
        const int parallelThreadsCount {testId % 5 + 1};
        auto gridSolver = GridSolverFactory::Make(parallelThreadsCount);

        auto grid = CreateGrid(gridSize, KeepRandomCells(positionsValues, cellsKept));

        try
        {
            const auto gridStatus = gridSolver->Solve(grid);

            //TODO [hypothesis]: check EXPECT_THAT(gridStatus, Eq(GridStatus::SolvedCorrectly));
            EXPECT_THAT(gridStatus, Ne(GridStatus::Wrong));
            if (gridStatus != GridStatus::Incomplete)
            {
                EXPECT_THAT(grid, Eq(expectedGrid));
            }
        }
        catch(std::exception& e)
        {
            FAIL() << "Couldn't solve grid because: " << e.what();
        }
    }
}

} /* namespace test */
} /* namespace sudoku */
