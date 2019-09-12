#include "RelatedPossibilitiesRemover.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "FoundPositions.hpp"
#include "Value.hpp"
#include "Grid.hpp"
#include "utils/Utils.hpp"

using testing::Eq;

namespace sudoku
{
namespace test
{

class TestRelatedPossibilitiesRemover : public ::testing::Test
{
public:
    TestRelatedPossibilitiesRemover()
    {}

    std::unique_ptr<RelatedPossibilitiesRemover> MakeRelatedPossibilitiesRemover()
    {
        return std::make_unique<RelatedPossibilitiesRemoverImpl>();
    }

    Grid CreateExpectedGridWithValuePos1x1SetTo(Value value)
    {
        Grid grid(m_GridSize);

        grid.GetCell(Position{1, 1}).SetValue(value);
        grid.GetCell(Position{1, 0}).RemovePossibility(value);
        grid.GetCell(Position{1, 2}).RemovePossibility(value);
        grid.GetCell(Position{1, 3}).RemovePossibility(value);
        grid.GetCell(Position{0, 1}).RemovePossibility(value);
        grid.GetCell(Position{2, 1}).RemovePossibility(value);
        grid.GetCell(Position{3, 1}).RemovePossibility(value);
        grid.GetCell(Position{0, 0}).RemovePossibility(value);

        return grid;
    }

    const int m_GridSize {4};
};

TEST_F(TestRelatedPossibilitiesRemover, RemoveSetValueFromRelatedCells)
{
    const Value cellValue {3};

    Grid grid {m_GridSize};
    FoundPositions foundPositions;

    Position currentPosition {1, 1};
    auto& cell = grid.GetCell(currentPosition);
    cell.SetValue(cellValue);

    MakeRelatedPossibilitiesRemover()->UpdateRelatedPossibilities(currentPosition, grid, foundPositions);

    auto expectedGrid = CreateExpectedGridWithValuePos1x1SetTo(cellValue);

    EXPECT_THAT(grid, Eq(expectedGrid));

    EXPECT_TRUE(foundPositions.empty());
}

TEST_F(TestRelatedPossibilitiesRemover, RemoveSetValueFromRelatedCells_RelatedCellNotIdentical)
{
    const Value cellValue {3};

    Grid grid {m_GridSize};
    FoundPositions foundPositions;

    Position currentPosition {1, 1};
    auto& cell = grid.GetCell(currentPosition);
    cell.SetValue(cellValue);

    const Value otherCellFoundValue {1};
    grid.GetCell(Position{0, 0}).SetValue(otherCellFoundValue);

    MakeRelatedPossibilitiesRemover()->UpdateRelatedPossibilities(currentPosition, grid, foundPositions);

    auto expectedGrid = CreateExpectedGridWithValuePos1x1SetTo(cellValue);
    expectedGrid.GetCell(Position{0, 0}).SetValue(otherCellFoundValue);

    EXPECT_THAT(grid, Eq(expectedGrid));

    EXPECT_TRUE(foundPositions.empty());
}

TEST_F(TestRelatedPossibilitiesRemover, RemoveSetValueFromRelatedCells_RelatedCellAlreadySetWithSameValue)
{
    const Value cellValue {3};

    Grid grid {m_GridSize};
    FoundPositions foundPositions;

    Position currentPosition {1, 1};
    auto& cell = grid.GetCell(currentPosition);
    cell.SetValue(cellValue);

    grid.GetCell(Position{0, 0}).SetValue(cellValue);

    EXPECT_THROW(MakeRelatedPossibilitiesRemover()->UpdateRelatedPossibilities(currentPosition, grid, foundPositions), std::exception);
}

TEST_F(TestRelatedPossibilitiesRemover, RemoveSetValueFromRelatedCells_FindNewCellValue)
{
    const Value cellValue {3};

    Grid grid {m_GridSize};
    FoundPositions foundPositions;

    Position currentPosition {1, 1};
    auto& cell = grid.GetCell(currentPosition);
    cell.SetValue(cellValue);

    const Value NewFoundValue {2};
    RemoveAllCellPossibilitiesBut(grid.GetCell(Position{1, 3}), {cellValue, NewFoundValue});

    MakeRelatedPossibilitiesRemover()->UpdateRelatedPossibilities(currentPosition, grid, foundPositions);

    auto expectedGrid = CreateExpectedGridWithValuePos1x1SetTo(cellValue);
    expectedGrid.GetCell(Position{1, 3}).SetValue(NewFoundValue);

    EXPECT_THAT(grid, Eq(expectedGrid));

    auto pushedPositions = QueueToVector(foundPositions);
    EXPECT_THAT(pushedPositions, Eq(std::vector<Position>{Position {1, 3}}));
}

TEST_F(TestRelatedPossibilitiesRemover, RemoveSetValueFromRelatedCells_CellValueNotSet)
{
    Grid grid {m_GridSize};
    FoundPositions foundPositions;

    Position currentPosition {1, 1};

    EXPECT_THROW(MakeRelatedPossibilitiesRemover()->UpdateRelatedPossibilities(currentPosition, grid, foundPositions), std::exception);
}

} /* namespace test */
} /* namespace sudoku */
