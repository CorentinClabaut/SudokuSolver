#include "RelatedPossibilitiesRemover.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "utils/Utils.hpp"

#include "FoundPositions.hpp"

#include "mock/MockRelatedPositionsGetter.hpp"

using testing::_;
using testing::Eq;
using testing::Ref;
using testing::Return;
using testing::ReturnRef;
using testing::StrictMock;

namespace sudoku
{
namespace test
{

class TestRelatedPossibilitiesRemover : public ::testing::Test
{
public:
    TestRelatedPossibilitiesRemover()
    {}

    void SetupCel1x1RelatedPositionsGetter()
    {
        static const std::array<Position, 7> m_1x1AllRelatedPositions {
            Position{1, 0},
            Position{1, 2},
            Position{1, 3},
            Position{0, 1},
            Position{2, 1},
            Position{3, 1},
            Position{0, 0}};

        EXPECT_CALL(*m_RelatedPositionsGetter, GetAllRelatedPositions(Position{1, 1}, 4))
                .WillRepeatedly(Return(Range{m_1x1AllRelatedPositions}));
    }

    std::unique_ptr<RelatedPossibilitiesRemover> MakeRelatedPossibilitiesRemover()
    {
        return std::make_unique<RelatedPossibilitiesRemoverImpl>(std::move(m_RelatedPositionsGetter));
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

    std::unique_ptr<MockRelatedPositionsGetter> m_RelatedPositionsGetter = std::make_unique<MockRelatedPositionsGetter>();
};

TEST_F(TestRelatedPossibilitiesRemover, RemoveSetValueFromRelatedCells)
{
    const Value cellValue {3};

    Grid grid {m_GridSize};
    FoundPositions foundPositions;

    SetupCel1x1RelatedPositionsGetter();

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

    SetupCel1x1RelatedPositionsGetter();

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

    SetupCel1x1RelatedPositionsGetter();

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

    SetupCel1x1RelatedPositionsGetter();

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

    SetupCel1x1RelatedPositionsGetter();

    Position currentPosition {1, 1};

    EXPECT_THROW(MakeRelatedPossibilitiesRemover()->UpdateRelatedPossibilities(currentPosition, grid, foundPositions), std::exception);
}

} /* namespace test */
} /* namespace sudoku */
