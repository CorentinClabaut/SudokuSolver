#include "GridSolverWithHypothesis.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "FoundCells.hpp"
#include "GridStatus.hpp"
#include "Grid.hpp"

#include "mock/MockGridSolverWithoutHypothesis.hpp"

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

class TestGridSolverWithHypothesis : public ::testing::Test
{
public:
    TestGridSolverWithHypothesis()
    {}

    std::unique_ptr<GridSolver> MakeGridSolverWithHypothesis()
    {
        return std::make_unique<GridSolverWithHypothesisImpl>(
                    std::move(m_GridSolverWithoutHypothesis));
    }

    std::unique_ptr<MockGridSolverWithoutHypothesis> m_GridSolverWithoutHypothesis = std::make_unique<StrictMock<MockGridSolverWithoutHypothesis>>();
};

TEST_F(TestGridSolverWithHypothesis, GridSolvedWithoutHypthesis)
{
    const int gridSize {4};
    Grid grid {gridSize};

    EXPECT_CALL(*m_GridSolverWithoutHypothesis, Solve(Ref(grid), _)).WillOnce(Return(GridStatus::SolvedCorrectly));

    auto correctlySolved = MakeGridSolverWithHypothesis()->Solve(grid);

    EXPECT_TRUE(correctlySolved);
}

TEST_F(TestGridSolverWithHypothesis, GridSolvedWrongBeforeHypthesis)
{
    const int gridSize {4};
    Grid grid {gridSize};

    EXPECT_CALL(*m_GridSolverWithoutHypothesis, Solve(Ref(grid), _)).WillOnce(Return(GridStatus::Wrong));

    auto correctlySolved = MakeGridSolverWithHypothesis()->Solve(grid);

    EXPECT_FALSE(correctlySolved);
}

TEST_F(TestGridSolverWithHypothesis, GridSolvedAfterOneHypthesisOnCell)
{
    const int gridSize {4};
    Grid grid {gridSize};

    Position hypothesisCellPosition {1, 2};

    grid.GetCell(Position {0, 1})->SetValue(4);
    grid.GetCell(Position {3, 2})->SetValue(2);
    grid.GetCell(Position {1, 0})->RemovePossibility(3);
    grid.GetCell(hypothesisCellPosition)->RemovePossibility(4);
    grid.GetCell(hypothesisCellPosition)->RemovePossibility(3);

    Grid hypothesisGrid {grid};
    hypothesisGrid.GetCell(hypothesisCellPosition)->SetValue(2);

    {
    InSequence s;
    EXPECT_CALL(*m_GridSolverWithoutHypothesis, Solve(Ref(grid), _)).WillOnce(Return(GridStatus::Incomplete));
    EXPECT_CALL(*m_GridSolverWithoutHypothesis, Solve(hypothesisGrid, _)).WillOnce(Return(GridStatus::SolvedCorrectly));
    }

    auto correctlySolved = MakeGridSolverWithHypothesis()->Solve(grid);

    EXPECT_TRUE(correctlySolved);
    EXPECT_THAT(grid, Eq(hypothesisGrid));
}

TEST_F(TestGridSolverWithHypothesis, GridSolvedAfterSeveralHypthesisOnCell)
{
    const int gridSize {4};
    Grid grid {gridSize};

    Position hypothesisCellPosition {1, 2};

    grid.GetCell(Position {0, 1})->SetValue(4);
    grid.GetCell(Position {3, 2})->SetValue(2);
    grid.GetCell(Position {1, 0})->RemovePossibility(3);
    grid.GetCell(hypothesisCellPosition)->RemovePossibility(4);
    grid.GetCell(hypothesisCellPosition)->RemovePossibility(3);

    Grid hypothesisGrid1 {grid};
    hypothesisGrid1.GetCell(hypothesisCellPosition)->SetValue(2);

    Grid hypothesisGrid2 {grid};
    hypothesisGrid2.GetCell(hypothesisCellPosition)->SetValue(1);

    {
    InSequence s;
    EXPECT_CALL(*m_GridSolverWithoutHypothesis, Solve(Ref(grid), _)).WillOnce(Return(GridStatus::Incomplete));
    EXPECT_CALL(*m_GridSolverWithoutHypothesis, Solve(hypothesisGrid1, _)).WillOnce(Return(GridStatus::Wrong));
    EXPECT_CALL(*m_GridSolverWithoutHypothesis, Solve(hypothesisGrid2, _)).WillOnce(Return(GridStatus::SolvedCorrectly));
    }

    auto correctlySolved = MakeGridSolverWithHypothesis()->Solve(grid);

    EXPECT_TRUE(correctlySolved);
    EXPECT_THAT(grid, Eq(hypothesisGrid2));
}

TEST_F(TestGridSolverWithHypothesis, GridSolvedWrongAfterSeveralHypthesisOnCell)
{
    const int gridSize {4};
    Grid grid {gridSize};

    Position hypothesisCellPosition {1, 2};

    grid.GetCell(Position {0, 1})->SetValue(4);
    grid.GetCell(Position {3, 2})->SetValue(2);
    grid.GetCell(Position {1, 0})->RemovePossibility(3);
    grid.GetCell(hypothesisCellPosition)->RemovePossibility(4);
    grid.GetCell(hypothesisCellPosition)->RemovePossibility(3);

    Grid hypothesisGrid1 {grid};
    hypothesisGrid1.GetCell(hypothesisCellPosition)->SetValue(2);

    Grid hypothesisGrid2 {grid};
    hypothesisGrid2.GetCell(hypothesisCellPosition)->SetValue(1);

    {
    InSequence s;
    EXPECT_CALL(*m_GridSolverWithoutHypothesis, Solve(Ref(grid), _)).WillOnce(Return(GridStatus::Incomplete));
    EXPECT_CALL(*m_GridSolverWithoutHypothesis, Solve(hypothesisGrid1, _)).WillOnce(Return(GridStatus::Wrong));
    EXPECT_CALL(*m_GridSolverWithoutHypothesis, Solve(hypothesisGrid2, _)).WillOnce(Return(GridStatus::Wrong));
    }

    auto correctlySolved = MakeGridSolverWithHypothesis()->Solve(grid);

    EXPECT_FALSE(correctlySolved);
}

TEST_F(TestGridSolverWithHypothesis, GridSolvedAfterSeveralHypthesisOnDifferentCells)
{
    const int gridSize {4};
    Grid grid {gridSize};

    Position hypothesis1CellPosition {1, 2};
    Position hypothesis2CellPosition {1, 0};

    grid.GetCell(Position {0, 1})->SetValue(4);
    grid.GetCell(Position {3, 2})->SetValue(2);
    grid.GetCell(hypothesis2CellPosition)->RemovePossibility(4);
    grid.GetCell(hypothesis1CellPosition)->RemovePossibility(4);
    grid.GetCell(hypothesis1CellPosition)->RemovePossibility(3);

    Grid hypothesisGrid1 {grid};
    hypothesisGrid1.GetCell(hypothesis1CellPosition)->SetValue(2);

    Grid hypothesisGrid2 {hypothesisGrid1};
    hypothesisGrid2.GetCell(hypothesis2CellPosition)->SetValue(3);

    {
    InSequence s;
    EXPECT_CALL(*m_GridSolverWithoutHypothesis, Solve(Ref(grid), _)).WillOnce(Return(GridStatus::Incomplete));
    EXPECT_CALL(*m_GridSolverWithoutHypothesis, Solve(hypothesisGrid1, _)).WillOnce(Return(GridStatus::Incomplete));
    EXPECT_CALL(*m_GridSolverWithoutHypothesis, Solve(hypothesisGrid2, _)).WillOnce(Return(GridStatus::SolvedCorrectly));
    }

    auto correctlySolved = MakeGridSolverWithHypothesis()->Solve(grid);

    EXPECT_TRUE(correctlySolved);
    EXPECT_THAT(grid, Eq(hypothesisGrid2));
}


} /* namespace test */
} /* namespace sudoku */
