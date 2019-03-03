#include "GridSolver.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "FoundCells.hpp"
#include "GridStatus.hpp"
#include "Grid.hpp"

#include "mock/MockParallelPossibilitiesRemover.hpp"
#include "mock/MockParallelUniquePossibilitySetter.hpp"
#include "mock/MockGridStatusGetter.hpp"
#include "utils/Utils.hpp"

using testing::_;
using testing::Eq;
using testing::Ref;
using testing::Return;
using testing::Invoke;
using testing::StrictMock;
using ::testing::InSequence;

namespace sudoku
{
namespace test
{

class TestGridSolver : public ::testing::Test
{
public:
    TestGridSolver()
    {}

    std::unique_ptr<GridSolver> MakeGridSolver()
    {
        return std::make_unique<GridSolverImpl>(
                    std::move(m_ParallelPossibilitiesRemover),
                    std::move(m_ParallelUniquePossibilitySetter),
                    std::move(m_GridStatusGetter));
    }

    void ExpectGridStatus(Grid const& grid, GridStatus gridStatus)
    {
        EXPECT_CALL(*m_GridStatusGetter, GetStatus(Ref(grid))).WillOnce(Return(gridStatus));
    }

    void ExpectUpdateGrid(Grid& grid)
    {
        EXPECT_CALL(*m_ParallelPossibilitiesRemover, UpdateGrid(_, Ref(grid))).Times(1);
    }

    void ExpectSetCellsWithUniquePossibility_FoundCells(Grid& grid, SharedCell newFoundCell)
    {
        EXPECT_CALL(*m_ParallelUniquePossibilitySetter, SetCellsWithUniquePossibility(Ref(grid), _))
                .WillOnce(Invoke([newFoundCell](Grid&, FoundCells& foundCells){ foundCells.m_Queue.push(newFoundCell); }));
    }

    void ExpectSetCellsWithUniquePossibility_NoCellFound(Grid& grid)
    {
        EXPECT_CALL(*m_ParallelUniquePossibilitySetter, SetCellsWithUniquePossibility(Ref(grid), _))
                .WillOnce(Invoke([](Grid&, FoundCells& foundCells){ while(!foundCells.m_Queue.empty()) { foundCells.m_Queue.pop();} }));
    }

    std::unique_ptr<MockParallelPossibilitiesRemover> m_ParallelPossibilitiesRemover = std::make_unique<StrictMock<MockParallelPossibilitiesRemover>>();
    std::unique_ptr<MockParallelUniquePossibilitySetter> m_ParallelUniquePossibilitySetter = std::make_unique<StrictMock<MockParallelUniquePossibilitySetter>>();
    std::unique_ptr<MockGridStatusGetter> m_GridStatusGetter = std::make_unique<StrictMock<MockGridStatusGetter>>();
};

TEST_F(TestGridSolver, EmptyGridIn)
{
    const int gridSize {4};
    Grid grid {gridSize};

    auto gridStatus = MakeGridSolver()->Solve(grid);

    EXPECT_THAT(gridStatus, Eq(GridStatus::Incomplete));
}

TEST_F(TestGridSolver, CorrectlySolvedGridIn)
{
    Grid grid = Create4x4CorrectlySolvedGrid();

    ExpectGridStatus(grid, GridStatus::SolvedCorrectly);

    auto gridStatus = MakeGridSolver()->Solve(grid);

    EXPECT_THAT(gridStatus, Eq(GridStatus::SolvedCorrectly));
}

TEST_F(TestGridSolver, IncorrectlySolvedGridIn)
{
    Grid grid = Create4x4IncorrectlySolvedGrid();

    ExpectGridStatus(grid, GridStatus::Wrong);

    auto gridStatus = MakeGridSolver()->Solve(grid);

    EXPECT_THAT(gridStatus, Eq(GridStatus::Wrong));
}

TEST_F(TestGridSolver, CorrectlySolvedAfterUpdateGrid)
{
    Grid grid = Create4x4CorrectlyPartiallyFilledGrid();

    {
    InSequence s;
    ExpectGridStatus(grid, GridStatus::Incomplete);
    ExpectUpdateGrid(grid);
    ExpectGridStatus(grid, GridStatus::SolvedCorrectly);
    }

    auto gridStatus = MakeGridSolver()->Solve(grid);

    EXPECT_THAT(gridStatus, Eq(GridStatus::SolvedCorrectly));
}

TEST_F(TestGridSolver, IncorrectlySolvedAfterUpdateGrid)
{
    Grid grid = Create4x4CorrectlyPartiallyFilledGrid();

    {
    InSequence s;
    ExpectGridStatus(grid, GridStatus::Incomplete);
    ExpectUpdateGrid(grid);
    ExpectGridStatus(grid, GridStatus::Wrong);
    }

    auto gridStatus = MakeGridSolver()->Solve(grid);

    EXPECT_THAT(gridStatus, Eq(GridStatus::Wrong));
}

TEST_F(TestGridSolver, CorrectlySolvedAfterSetCellsWithUniquePossibility)
{
    Grid grid = Create4x4CorrectlyPartiallyFilledGrid();

    {
    InSequence s;
    ExpectGridStatus(grid, GridStatus::Incomplete);
    ExpectUpdateGrid(grid);
    ExpectGridStatus(grid, GridStatus::Incomplete);
    ExpectSetCellsWithUniquePossibility_FoundCells(grid, grid.m_Cells[1][1]);
    ExpectGridStatus(grid, GridStatus::SolvedCorrectly);
    }

    auto gridStatus = MakeGridSolver()->Solve(grid);

    EXPECT_THAT(gridStatus, Eq(GridStatus::SolvedCorrectly));
}

TEST_F(TestGridSolver, IncorrectlySolvedAfterSetCellsWithUniquePossibility)
{
    Grid grid = Create4x4CorrectlyPartiallyFilledGrid();

    {
    InSequence s;
    ExpectGridStatus(grid, GridStatus::Incomplete);
    ExpectUpdateGrid(grid);
    ExpectGridStatus(grid, GridStatus::Incomplete);
    ExpectSetCellsWithUniquePossibility_FoundCells(grid, grid.m_Cells[1][1]);
    ExpectGridStatus(grid, GridStatus::Wrong);
    }

    auto gridStatus = MakeGridSolver()->Solve(grid);

    EXPECT_THAT(gridStatus, Eq(GridStatus::Wrong));
}

TEST_F(TestGridSolver, CantCompleteGrid)
{
    Grid grid = Create4x4CorrectlyPartiallyFilledGrid();

    {
    InSequence s;
    ExpectGridStatus(grid, GridStatus::Incomplete);
    ExpectUpdateGrid(grid);
    ExpectGridStatus(grid, GridStatus::Incomplete);
    ExpectSetCellsWithUniquePossibility_NoCellFound(grid);
    }

    auto gridStatus = MakeGridSolver()->Solve(grid);

    EXPECT_THAT(gridStatus, Eq(GridStatus::Incomplete));
}

} /* namespace test */
} /* namespace sudoku */
