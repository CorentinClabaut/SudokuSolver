#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "GridSolverFactory.hpp"
#include "GridStatus.hpp"
#include "Grid.hpp"
#include "utils/Utils.hpp"
#include "GridStatusGetter.hpp"
#include "RelatedCellsGetter.hpp"

using testing::Eq;
using testing::Ne;

namespace sudoku
{
namespace test
{

class FTestGridSolver : public ::testing::Test
{
public:
    FTestGridSolver() :
        m_GridStatusGetter(std::make_unique<RelatedCellsGetterImpl>())
    {}

    GridStatusGetterImpl m_GridStatusGetter;
};

TEST_F(FTestGridSolver, Solve4x4)
{
    const int gridSize {4};
    const int cellsKept {8};

    const auto positionsValues = CreatePositionsValues4x4();

    const int testExecutionCount = 50;
    for(int testId : boost::irange(1, testExecutionCount + 1))
    {
        const int parallelThreadsCount {testId % 5 + 1};
        auto gridSolver = GridSolverFactory::Make(parallelThreadsCount);

        auto grid = CreateGrid(gridSize, KeepRandomCells(positionsValues, cellsKept));

        try
        {
            const auto solvedCorrectly = gridSolver->Solve(grid);
            EXPECT_TRUE(solvedCorrectly);

            auto gridStatus = m_GridStatusGetter.GetStatus(grid);
            EXPECT_THAT(gridStatus, Eq(GridStatus::SolvedCorrectly));
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

    const int testExecutionCount = 50;
    for(int testId : boost::irange(1, testExecutionCount + 1))
    {
        const int parallelThreadsCount {testId % 5 + 1};
        auto gridSolver = GridSolverFactory::Make(parallelThreadsCount);

        auto grid = CreateGrid(gridSize, KeepRandomCells(positionsValues, cellsKept));

        try
        {
            const auto solvedCorrectly = gridSolver->Solve(grid);
            EXPECT_TRUE(solvedCorrectly);

            auto gridStatus = m_GridStatusGetter.GetStatus(grid);
            EXPECT_THAT(gridStatus, Eq(GridStatus::SolvedCorrectly));
        }
        catch(std::exception& e)
        {
            FAIL() << "Couldn't solve grid because: " << e.what();
        }
    }
}

//TEST_F(FTestGridSolver, SolveWrong9x9)
//{
//    const int gridSize {9};
//    const int cellsKept {40};
//
//    const auto positionsValues = CreatePositionsValues9x9();
//
//    const int testExecutionCount = 20;
//    for(int testId : boost::irange(1, testExecutionCount + 1))
//    {
//        const int parallelThreadsCount {testId % 5 + 1};
//        auto gridSolver = GridSolverFactory::Make(parallelThreadsCount);
//
//        auto gridRandCells = KeepRandomCells(positionsValues, cellsKept);
//        gridRandCells.front().second = ((gridRandCells.front().second + 1) % gridSize) + 1;
//        auto grid = CreateGrid(gridSize, gridRandCells);
//
//        try
//        {
//            const auto gridStatus = gridSolver->Solve(grid);
//
//            EXPECT_THAT(gridStatus, Eq(GridStatus::Wrong));
//        }
//        catch(std::exception& e)
//        {
//            SUCCEED() << "Couldn't solve grid because: " << e.what();
//        }
//    }
//}

} /* namespace test */
} /* namespace sudoku */
