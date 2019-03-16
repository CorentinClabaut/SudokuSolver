#include "GridSolverWithoutHypothesis.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <exception>

#include "FoundCells.hpp"
#include "GridStatus.hpp"
#include "Grid.hpp"

#include "mock/MockParallelPossibilitiesRemover.hpp"
#include "mock/MockParallelUniquePossibilitySetter.hpp"
#include "utils/Utils.hpp"

using testing::_;
using testing::Eq;
using testing::Ref;
using testing::Throw;
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
                    std::move(m_ParallelUniquePossibilitySetter));
    }

    void ExpectUpdateGrid(Grid& grid)
    {
        EXPECT_CALL(*m_ParallelPossibilitiesRemover, UpdateGrid(_, Ref(grid)))
                .WillOnce(Invoke([](FoundCells& foundCells, Grid&){ while(!foundCells.m_Queue.empty()) { foundCells.m_Queue.pop();} }));
    }

    void ExpectUpdateGrid_SolveGrid(Grid& grid)
    {
        EXPECT_CALL(*m_ParallelPossibilitiesRemover, UpdateGrid(_, Ref(grid)))
                .WillOnce(Invoke([](FoundCells& foundCells, Grid& grid)
                    {
                        while(!foundCells.m_Queue.empty()) { foundCells.m_Queue.pop();}
                        for (auto cell : grid) { if (!cell->GetValue()) cell->SetValue(1); }
                    }));
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
};

TEST_F(TestGridSolverWithoutHypothesis, EmptyFoundCellsInThrow)
{
    const int gridSize {4};
    Grid grid {gridSize};

    EXPECT_THROW(MakeGridSolverWithoutHypothesis()->Solve(grid, m_FoundCells), std::exception);
}

TEST_F(TestGridSolverWithoutHypothesis, CouldntResolveIfRemovePossibilityFail)
{
    Grid grid = Create4x4CorrectlyPartiallyFilledGrid();
    m_FoundCells.m_Queue.push(grid.m_Cells[0][0]);

    EXPECT_CALL(*m_ParallelPossibilitiesRemover, UpdateGrid(_, Ref(grid)))
            .WillRepeatedly(Invoke([](FoundCells& foundCells, Grid& grid)
                {
                    foundCells.m_Queue.push(grid.m_Cells[2][3]);
                    throw std::runtime_error("exception");
                }));

    auto gridStatus = MakeGridSolverWithoutHypothesis()->Solve(grid, m_FoundCells);

    EXPECT_THAT(gridStatus, Eq(GridStatus::Wrong));
    EXPECT_TRUE(m_FoundCells.m_Queue.empty());
}

TEST_F(TestGridSolverWithoutHypothesis, CouldntResolveIfSetCellsWithUniquePossibilityThrow)
{
    Grid grid = Create4x4CorrectlyPartiallyFilledGrid();
    m_FoundCells.m_Queue.push(grid.m_Cells[0][0]);

    {
    InSequence s;
    ExpectUpdateGrid(grid);
    EXPECT_CALL(*m_ParallelUniquePossibilitySetter, SetCellsWithUniquePossibility(Ref(grid), _))
            .WillRepeatedly(Invoke([](Grid& grid, FoundCells& foundCells)
                {
                    foundCells.m_Queue.push(grid.m_Cells[2][3]);
                    throw std::runtime_error("exception");
                }));
    }

    auto gridStatus = MakeGridSolverWithoutHypothesis()->Solve(grid, m_FoundCells);

    EXPECT_THAT(gridStatus, Eq(GridStatus::Wrong));
    EXPECT_TRUE(m_FoundCells.m_Queue.empty());
}

TEST_F(TestGridSolverWithoutHypothesis, GridSolvedAfterFirstRemovePossibilities)
{
    Grid grid = Create4x4CorrectlyPartiallyFilledGrid();
    m_FoundCells.m_Queue.push(grid.m_Cells[0][0]);

    {
    InSequence s;
    ExpectUpdateGrid_SolveGrid(grid);
    ExpectSetCellsWithUniquePossibility_NoCellFound(grid);
    }

    auto gridStatus = MakeGridSolverWithoutHypothesis()->Solve(grid, m_FoundCells);

    EXPECT_THAT(gridStatus, Eq(GridStatus::SolvedCorrectly));
    EXPECT_TRUE(m_FoundCells.m_Queue.empty());
}

TEST_F(TestGridSolverWithoutHypothesis, GridNotSolvedAfterFirstRemovePossibilities)
{
    Grid grid = Create4x4CorrectlyPartiallyFilledGrid();
    m_FoundCells.m_Queue.push(grid.m_Cells[0][0]);

    {
    InSequence s;
    ExpectUpdateGrid(grid);
    ExpectSetCellsWithUniquePossibility_NoCellFound(grid);
    }

    auto gridStatus = MakeGridSolverWithoutHypothesis()->Solve(grid, m_FoundCells);

    EXPECT_THAT(gridStatus, Eq(GridStatus::Incomplete));
    EXPECT_TRUE(m_FoundCells.m_Queue.empty());
}

TEST_F(TestGridSolverWithoutHypothesis, GridSolvedAfterSecondRemovePossibilities)
{
    Grid grid = Create4x4CorrectlyPartiallyFilledGrid();
    m_FoundCells.m_Queue.push(grid.m_Cells[0][0]);

    {
    InSequence s;
    ExpectUpdateGrid(grid);
    ExpectSetCellsWithUniquePossibility_FoundCells(grid, grid.m_Cells[2][3]);
    ExpectUpdateGrid_SolveGrid(grid);
    ExpectSetCellsWithUniquePossibility_NoCellFound(grid);
    }

    auto gridStatus = MakeGridSolverWithoutHypothesis()->Solve(grid, m_FoundCells);

    EXPECT_THAT(gridStatus, Eq(GridStatus::SolvedCorrectly));
    EXPECT_TRUE(m_FoundCells.m_Queue.empty());
}

} /* namespace test */
} /* namespace sudoku */
