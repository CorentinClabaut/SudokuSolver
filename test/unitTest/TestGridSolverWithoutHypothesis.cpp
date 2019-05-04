#include "GridSolverWithoutHypothesis.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <exception>

#include "FoundPositions.hpp"
#include "GridStatus.hpp"
#include "Grid.hpp"

#include "mock/MockGridPossibilitiesUpdater.hpp"
#include "mock/MockUniquePossibilitySetter.hpp"
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
                    std::move(m_GridPossibilitiesUpdater),
                    std::move(m_UniquePossibilitySetter));
    }

    void ExpectUpdateGrid(Grid& grid)
    {
        EXPECT_CALL(*m_GridPossibilitiesUpdater, UpdateGrid(_, Ref(grid)))
                .WillOnce(Invoke([](FoundPositions& foundPositions, Grid&){ while(!foundPositions.empty()) { foundPositions.pop();} }));
    }

    void ExpectUpdateGrid_SolveGrid(Grid& grid)
    {
        EXPECT_CALL(*m_GridPossibilitiesUpdater, UpdateGrid(_, Ref(grid)))
                .WillOnce(Invoke([](FoundPositions& foundPositions, Grid& grid)
                    {
                        while(!foundPositions.empty()) { foundPositions.pop();}
                        for (auto& cell : grid) { if (!cell.GetValue()) cell.SetValue(1); }
                    }));
    }

    void ExpectSetCellsWithUniquePossibility_FoundPositions(Grid& grid, Position const& newFoundPosition)
    {
        EXPECT_CALL(*m_UniquePossibilitySetter, SetCellsWithUniquePossibility(Ref(grid), _))
                .WillOnce(Invoke([&newFoundPosition](Grid&, FoundPositions& foundPositions){ foundPositions.push(newFoundPosition); }));
    }

    void ExpectSetCellsWithUniquePossibility_NoCellFound(Grid& grid)
    {
        EXPECT_CALL(*m_UniquePossibilitySetter, SetCellsWithUniquePossibility(Ref(grid), _)).Times(1);
    }

    FoundPositions m_FoundPositions;

    std::unique_ptr<MockGridPossibilitiesUpdater> m_GridPossibilitiesUpdater = std::make_unique<StrictMock<MockGridPossibilitiesUpdater>>();
    std::unique_ptr<MockUniquePossibilitySetter> m_UniquePossibilitySetter = std::make_unique<StrictMock<MockUniquePossibilitySetter>>();
};

TEST_F(TestGridSolverWithoutHypothesis, EmptyFoundPositionsInThrow)
{
    const int gridSize {4};
    Grid grid {gridSize};

    EXPECT_THROW(MakeGridSolverWithoutHypothesis()->Solve(grid, m_FoundPositions), std::exception);
}

TEST_F(TestGridSolverWithoutHypothesis, CouldntResolveIfUpdateGridFail)
{
    Grid grid = Create4x4CorrectlyPartiallyFilledGrid();
    m_FoundPositions.push(Position {0, 0});

    EXPECT_CALL(*m_GridPossibilitiesUpdater, UpdateGrid(_, Ref(grid)))
            .WillRepeatedly(Invoke([](FoundPositions& foundPositions, Grid& grid)
                {
                    foundPositions.push(Position {2, 3});
                    throw std::runtime_error("exception");
                }));

    auto gridStatus = MakeGridSolverWithoutHypothesis()->Solve(grid, m_FoundPositions);

    EXPECT_THAT(gridStatus, Eq(GridStatus::Wrong));
    EXPECT_TRUE(m_FoundPositions.empty());
}

TEST_F(TestGridSolverWithoutHypothesis, CouldntResolveIfSetCellsWithUniquePossibilityThrow)
{
    Grid grid = Create4x4CorrectlyPartiallyFilledGrid();
    m_FoundPositions.push(Position {0, 0});

    {
    InSequence s;
    ExpectUpdateGrid(grid);
    EXPECT_CALL(*m_UniquePossibilitySetter, SetCellsWithUniquePossibility(Ref(grid), _))
            .WillRepeatedly(Invoke([](Grid& grid, FoundPositions& foundPositions)
                {
                    foundPositions.push(Position {2, 3});
                    throw std::runtime_error("exception");
                }));
    }

    auto gridStatus = MakeGridSolverWithoutHypothesis()->Solve(grid, m_FoundPositions);

    EXPECT_THAT(gridStatus, Eq(GridStatus::Wrong));
    EXPECT_TRUE(m_FoundPositions.empty());
}

TEST_F(TestGridSolverWithoutHypothesis, GridSolvedAfterFirstUpdateGrid)
{
    Grid grid = Create4x4CorrectlyPartiallyFilledGrid();
    m_FoundPositions.push(Position {0, 0});

    {
    InSequence s;
    ExpectUpdateGrid_SolveGrid(grid);
    ExpectSetCellsWithUniquePossibility_NoCellFound(grid);
    }

    auto gridStatus = MakeGridSolverWithoutHypothesis()->Solve(grid, m_FoundPositions);

    EXPECT_THAT(gridStatus, Eq(GridStatus::SolvedCorrectly));
    EXPECT_TRUE(m_FoundPositions.empty());
}

TEST_F(TestGridSolverWithoutHypothesis, GridNotSolvedAfterFirstUpdateGrid)
{
    Grid grid = Create4x4CorrectlyPartiallyFilledGrid();
    m_FoundPositions.push(Position {0, 0});

    {
    InSequence s;
    ExpectUpdateGrid(grid);
    ExpectSetCellsWithUniquePossibility_NoCellFound(grid);
    }

    auto gridStatus = MakeGridSolverWithoutHypothesis()->Solve(grid, m_FoundPositions);

    EXPECT_THAT(gridStatus, Eq(GridStatus::Incomplete));
    EXPECT_TRUE(m_FoundPositions.empty());
}

TEST_F(TestGridSolverWithoutHypothesis, GridSolvedAfterSecondUpdateGrid)
{
    Grid grid = Create4x4CorrectlyPartiallyFilledGrid();
    m_FoundPositions.push(Position {0, 0});

    {
    InSequence s;
    ExpectUpdateGrid(grid);
    ExpectSetCellsWithUniquePossibility_FoundPositions(grid, Position {2, 3});
    ExpectUpdateGrid_SolveGrid(grid);
    ExpectSetCellsWithUniquePossibility_NoCellFound(grid);
    }

    auto gridStatus = MakeGridSolverWithoutHypothesis()->Solve(grid, m_FoundPositions);

    EXPECT_THAT(gridStatus, Eq(GridStatus::SolvedCorrectly));
    EXPECT_TRUE(m_FoundPositions.empty());
}

} /* namespace test */
} /* namespace sudoku */
