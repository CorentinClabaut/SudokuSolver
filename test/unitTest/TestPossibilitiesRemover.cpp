#include "PossibilitiesRemover.hpp"

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

class TestPossibilitiesRemover : public ::testing::Test
{
public:
    TestPossibilitiesRemover()
    {}

    void SetupCel1x1RelatedPositionsGetter()
    {
        EXPECT_CALL(*m_RelatedPositionsGetter, GetAllRelatedPositions(Position{1, 1}, 4, 2))
                    .WillRepeatedly(Return(std::vector<Position>{{1, 0}, {1, 2}, {1, 3}, {0, 1}, {2, 1}, {3, 1}, {0, 0}}));
    }

    std::unique_ptr<PossibilitiesRemover> MakePossibilitiesRemover()
    {
        return std::make_unique<PossibilitiesRemoverImpl>(std::move(m_RelatedPositionsGetter));
    }

    template<typename T>
    void ExpectQueueEqual(std::queue<T>& queue, std::vector<T> const& vector)
    {
        EXPECT_THAT(queue.size(), Eq(vector.size()));

        auto vectorIter = vector.begin();
        while (!queue.empty())
        {
            auto val = queue.front();
            EXPECT_THAT(val, Eq(*vectorIter));
            queue.pop();

            vectorIter++;
        }
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

TEST_F(TestPossibilitiesRemover, RemoveSetValueFromRelatedCells)
{
    const Value cellValue {3};

    Grid grid {m_GridSize};
    FoundPositions foundPositions;

    SetupCel1x1RelatedPositionsGetter();

    Position currentPosition {1, 1};
    auto& cell = grid.GetCell(currentPosition);
    cell.SetValue(cellValue);

    MakePossibilitiesRemover()->UpdateGrid(currentPosition, grid, foundPositions);

    auto expectedGrid = CreateExpectedGridWithValuePos1x1SetTo(cellValue);

    EXPECT_THAT(grid, Eq(expectedGrid));

    EXPECT_TRUE(foundPositions.m_Queue.empty());
}

TEST_F(TestPossibilitiesRemover, RemoveSetValueFromRelatedCells_RelatedCellNotIdentical)
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

    MakePossibilitiesRemover()->UpdateGrid(currentPosition, grid, foundPositions);

    auto expectedGrid = CreateExpectedGridWithValuePos1x1SetTo(cellValue);
    expectedGrid.GetCell(Position{0, 0}).SetValue(otherCellFoundValue);

    EXPECT_THAT(grid, Eq(expectedGrid));

    EXPECT_TRUE(foundPositions.m_Queue.empty());
}

TEST_F(TestPossibilitiesRemover, RemoveSetValueFromRelatedCells_RelatedCellAlreadySetWithSameValue)
{
    const Value cellValue {3};

    Grid grid {m_GridSize};
    FoundPositions foundPositions;

    SetupCel1x1RelatedPositionsGetter();

    Position currentPosition {1, 1};
    auto& cell = grid.GetCell(currentPosition);
    cell.SetValue(cellValue);

    grid.GetCell(Position{0, 0}).SetValue(cellValue);

    EXPECT_THROW(MakePossibilitiesRemover()->UpdateGrid(currentPosition, grid, foundPositions), std::exception);
}

TEST_F(TestPossibilitiesRemover, RemoveSetValueFromRelatedCells_FindNewCellValue)
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

    MakePossibilitiesRemover()->UpdateGrid(currentPosition, grid, foundPositions);

    auto expectedGrid = CreateExpectedGridWithValuePos1x1SetTo(cellValue);
    expectedGrid.GetCell(Position{1, 3}).SetValue(NewFoundValue);

    EXPECT_THAT(grid, Eq(expectedGrid));

    ExpectQueueEqual(foundPositions.m_Queue, {Position {1, 3}});
}

TEST_F(TestPossibilitiesRemover, RemoveSetValueFromRelatedCells_CellValueNotSet)
{
    Grid grid {m_GridSize};
    FoundPositions foundPositions;

    SetupCel1x1RelatedPositionsGetter();

    Position currentPosition {1, 1};

    EXPECT_THROW(MakePossibilitiesRemover()->UpdateGrid(currentPosition, grid, foundPositions), std::exception);
}

} /* namespace test */
} /* namespace sudoku */
