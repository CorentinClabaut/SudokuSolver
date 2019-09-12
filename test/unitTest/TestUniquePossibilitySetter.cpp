#include "UniquePossibilitySetter.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Grid.hpp"
#include "utils/Utils.hpp"

using testing::Eq;

namespace sudoku
{
namespace test
{

class TestUniquePossibilitySetter : public ::testing::Test
{
public:
    void RemovePossibilityFromRelatedCol(Grid& grid, Position positionWithUniqueValue, Value possibility)
    {
        for (int i = 0; i < grid.GetGridSize(); i++)
        {
            Position position {positionWithUniqueValue.m_Row, i};

            if (position == positionWithUniqueValue)
                continue;

            grid.GetCell(position).RemovePossibility(possibility);
        }
    }

    FoundPositions m_FoundPositions;

    UniquePossibilitySetterImpl m_UniquePossibilitySetter;
};

TEST_F(TestUniquePossibilitySetter, NoUniquePossibility)
{
    const int gridSize {4};
    Grid grid {gridSize};

    m_UniquePossibilitySetter.SetCellsWithUniquePossibility(grid, m_FoundPositions);

    EXPECT_TRUE(m_FoundPositions.empty());
    EXPECT_THAT(grid, Eq(Grid {gridSize}));
}

TEST_F(TestUniquePossibilitySetter, UniquePossibility)
{
    const int gridSize {4};
    Grid grid {gridSize};

    const Value newFoundValue {1};
    Position positionWithUniqueValue {1, 0};

    RemovePossibilityFromRelatedCol(grid, positionWithUniqueValue, newFoundValue);

    auto expectedGrid = grid;
    expectedGrid.GetCell(positionWithUniqueValue).SetValue(newFoundValue);

    m_UniquePossibilitySetter.SetCellsWithUniquePossibility(grid, m_FoundPositions);

    EXPECT_THAT(QueueToVector(m_FoundPositions), Eq(std::vector<Position>{positionWithUniqueValue}));

    EXPECT_THAT(grid, Eq(expectedGrid));
}

TEST_F(TestUniquePossibilitySetter, TwoUniquePossibilitiesInGroup)
{
    const int gridSize {4};
    Grid grid {gridSize};

    const Value newFoundValue1 {3};
    Position positionWithUniqueValue1 {1, 0};

    const Value newFoundValue2 {1};
    Position positionWithUniqueValue2 {1, 2};

    RemovePossibilityFromRelatedCol(grid, positionWithUniqueValue1, newFoundValue1);
    RemovePossibilityFromRelatedCol(grid, positionWithUniqueValue2, newFoundValue2);

    auto expectedGrid = grid;
    expectedGrid.GetCell(positionWithUniqueValue1).SetValue(newFoundValue1);
    expectedGrid.GetCell(positionWithUniqueValue2).SetValue(newFoundValue2);

    m_UniquePossibilitySetter.SetCellsWithUniquePossibility(grid, m_FoundPositions);

    EXPECT_THAT(QueueToVector(m_FoundPositions), Eq(std::vector<Position>{positionWithUniqueValue1, positionWithUniqueValue2}));

    EXPECT_THAT(grid, Eq(expectedGrid));
}

TEST_F(TestUniquePossibilitySetter, SeveralUniquePossibilitiesInSameCellsThrow)
{
    const int gridSize {4};
    Grid grid {gridSize};

    Position positionWithUniqueValue1 {1, 0};

    RemovePossibilityFromRelatedCol(grid, positionWithUniqueValue1, 1);
    RemovePossibilityFromRelatedCol(grid, positionWithUniqueValue1, 3);

    EXPECT_THROW( m_UniquePossibilitySetter.SetCellsWithUniquePossibility(grid, m_FoundPositions), std::exception);
}

} /* namespace test */
} /* namespace sudoku */
