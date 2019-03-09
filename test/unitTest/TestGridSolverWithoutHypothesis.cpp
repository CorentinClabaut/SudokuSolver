#include "GridSolverWithoutHypothesis.hpp"

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

class TestGridSolverWithoutHypothesis : public ::testing::Test
{
public:
    TestGridSolverWithoutHypothesis()
    {}

    std::unique_ptr<GridSolverWithoutHypothesis> MakeGridSolverWithoutHypothesis()
    {
        return std::make_unique<GridSolverWithoutHypothesisImpl>(
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
        EXPECT_CALL(*m_ParallelPossibilitiesRemover, UpdateGrid(_, Ref(grid)))
                .WillOnce(Invoke([](FoundCells& foundCells, Grid&){ while(!foundCells.m_Queue.empty()) { foundCells.m_Queue.pop();} }));
    }

    void ExpectSetCellsWithUniquePossibility_FoundCells(Grid& grid, SharedCell newFoundCell)
    {
        EXPECT_CALL(*m_ParallelUniquePossibilitySetter, SetCellsWithUniquePossibility(Ref(grid), _))
                .WillOnce(Invoke([newFoundCell](Grid&, FoundCells& foundCells){ foundCells.m_Queue.push(newFoundCell); }));
    }

    void ExpectSetCellsWithUniquePossibility_NoCellFound(Grid& grid)
    {
        EXPECT_CALL(*m_ParallelUniquePossibilitySetter, SetCellsWithUniquePossibility(Ref(grid), _)).Times(1);
    }

    FoundCells m_FoundCells;

    std::unique_ptr<MockParallelPossibilitiesRemover> m_ParallelPossibilitiesRemover = std::make_unique<StrictMock<MockParallelPossibilitiesRemover>>();
    std::unique_ptr<MockParallelUniquePossibilitySetter> m_ParallelUniquePossibilitySetter = std::make_unique<StrictMock<MockParallelUniquePossibilitySetter>>();
    std::unique_ptr<MockGridStatusGetter> m_GridStatusGetter = std::make_unique<StrictMock<MockGridStatusGetter>>();
};

TEST_F(TestGridSolverWithoutHypothesis, EmptyGridIn)
{
    const int gridSize {4};
    Grid grid {gridSize};

    auto gridStatus = MakeGridSolverWithoutHypothesis()->Solve(grid, m_FoundCells);

    EXPECT_THAT(gridStatus, Eq(GridStatus::Incomplete));
}

TEST_F(TestGridSolverWithoutHypothesis, CorrectlySolvedGridIn)
{
    Grid grid = Create4x4CorrectlySolvedGrid();
    m_FoundCells.m_Queue.push(grid.m_Cells[0][0]);

    ExpectGridStatus(grid, GridStatus::SolvedCorrectly);

    auto gridStatus = MakeGridSolverWithoutHypothesis()->Solve(grid, m_FoundCells);

    EXPECT_THAT(gridStatus, Eq(GridStatus::SolvedCorrectly));
}

TEST_F(TestGridSolverWithoutHypothesis, IncorrectlySolvedGridIn)
{
    Grid grid = Create4x4IncorrectlySolvedGrid();
    m_FoundCells.m_Queue.push(grid.m_Cells[0][0]);

    ExpectGridStatus(grid, GridStatus::Wrong);

    auto gridStatus = MakeGridSolverWithoutHypothesis()->Solve(grid, m_FoundCells);

    EXPECT_THAT(gridStatus, Eq(GridStatus::Wrong));
}

TEST_F(TestGridSolverWithoutHypothesis, CorrectlySolvedAfterUpdateGrid)
{
    Grid grid = Create4x4CorrectlyPartiallyFilledGrid();
    m_FoundCells.m_Queue.push(grid.m_Cells[0][0]);

    {
    InSequence s;
    ExpectGridStatus(grid, GridStatus::Incomplete);
    ExpectUpdateGrid(grid);
    ExpectGridStatus(grid, GridStatus::SolvedCorrectly);
    }

    auto gridStatus = MakeGridSolverWithoutHypothesis()->Solve(grid, m_FoundCells);

    EXPECT_THAT(gridStatus, Eq(GridStatus::SolvedCorrectly));
}

TEST_F(TestGridSolverWithoutHypothesis, IncorrectlySolvedAfterUpdateGrid)
{
    Grid grid = Create4x4CorrectlyPartiallyFilledGrid();
    m_FoundCells.m_Queue.push(grid.m_Cells[0][0]);

    {
    InSequence s;
    ExpectGridStatus(grid, GridStatus::Incomplete);
    ExpectUpdateGrid(grid);
    ExpectGridStatus(grid, GridStatus::Wrong);
    }

    auto gridStatus = MakeGridSolverWithoutHypothesis()->Solve(grid, m_FoundCells);

    EXPECT_THAT(gridStatus, Eq(GridStatus::Wrong));
}

TEST_F(TestGridSolverWithoutHypothesis, CorrectlySolvedAfterSetCellsWithUniquePossibility)
{
    Grid grid = Create4x4CorrectlyPartiallyFilledGrid();
    m_FoundCells.m_Queue.push(grid.m_Cells[0][0]);

    {
    InSequence s;
    ExpectGridStatus(grid, GridStatus::Incomplete);
    ExpectUpdateGrid(grid);
    ExpectGridStatus(grid, GridStatus::Incomplete);
    ExpectSetCellsWithUniquePossibility_FoundCells(grid, grid.m_Cells[1][1]);
    ExpectGridStatus(grid, GridStatus::SolvedCorrectly);
    }

    auto gridStatus = MakeGridSolverWithoutHypothesis()->Solve(grid, m_FoundCells);

    EXPECT_THAT(gridStatus, Eq(GridStatus::SolvedCorrectly));
}

TEST_F(TestGridSolverWithoutHypothesis, IncorrectlySolvedAfterSetCellsWithUniquePossibility)
{
    Grid grid = Create4x4CorrectlyPartiallyFilledGrid();
    m_FoundCells.m_Queue.push(grid.m_Cells[0][0]);

    {
    InSequence s;
    ExpectGridStatus(grid, GridStatus::Incomplete);
    ExpectUpdateGrid(grid);
    ExpectGridStatus(grid, GridStatus::Incomplete);
    ExpectSetCellsWithUniquePossibility_FoundCells(grid, grid.m_Cells[1][1]);
    ExpectGridStatus(grid, GridStatus::Wrong);
    }

    auto gridStatus = MakeGridSolverWithoutHypothesis()->Solve(grid, m_FoundCells);

    EXPECT_THAT(gridStatus, Eq(GridStatus::Wrong));
}

TEST_F(TestGridSolverWithoutHypothesis, CantCompleteGrid)
{
    Grid grid = Create4x4CorrectlyPartiallyFilledGrid();
    m_FoundCells.m_Queue.push(grid.m_Cells[0][0]);

    {
    InSequence s;
    ExpectGridStatus(grid, GridStatus::Incomplete);
    ExpectUpdateGrid(grid);
    ExpectGridStatus(grid, GridStatus::Incomplete);
    ExpectSetCellsWithUniquePossibility_NoCellFound(grid);
    }

    auto gridStatus = MakeGridSolverWithoutHypothesis()->Solve(grid, m_FoundCells);

    EXPECT_THAT(gridStatus, Eq(GridStatus::Incomplete));
}

} /* namespace test */
} /* namespace sudoku */
