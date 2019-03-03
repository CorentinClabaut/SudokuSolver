#include "PossibilitiesRemover.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "utils/Utils.hpp"

#include "FoundCells.hpp"

#include "mock/MockRelatedCellsGetter.hpp"

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

    void SetupCel1x1RelatedCellsGetter(Grid& grid)
    {
        m_RelatedCellsGetter->ExpectGetAllRelatedCells(grid, {1, 1}, {{1, 0}, {1, 2}, {1, 3}, {0, 1}, {2, 1}, {3, 1}, {0, 0}});
    }

    std::unique_ptr<PossibilitiesRemover> MakePossibilitiesRemover()
    {
        return std::make_unique<PossibilitiesRemoverImpl>(std::move(m_RelatedCellsGetter));
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

        grid.m_Cells[1][1]->SetValue(value);
        grid.m_Cells[1][0]->RemovePossibility(value);
        grid.m_Cells[1][2]->RemovePossibility(value);
        grid.m_Cells[1][3]->RemovePossibility(value);
        grid.m_Cells[0][1]->RemovePossibility(value);
        grid.m_Cells[2][1]->RemovePossibility(value);
        grid.m_Cells[3][1]->RemovePossibility(value);
        grid.m_Cells[0][0]->RemovePossibility(value);

        return grid;
    }

    const int m_GridSize {4};

    std::unique_ptr<MockRelatedCellsGetter> m_RelatedCellsGetter = std::make_unique<MockRelatedCellsGetter>();
};

TEST_F(TestPossibilitiesRemover, RemoveSetValueFromRelatedCells)
{
    const Value cellValue {3};

    Grid grid {m_GridSize};
    FoundCells foundCells;

    SetupCel1x1RelatedCellsGetter(grid);

    auto cell = grid.m_Cells[1][1];
    cell->SetValue(cellValue);

    MakePossibilitiesRemover()->UpdateGrid(*cell, grid, foundCells);

    auto expectedGrid = CreateExpectedGridWithValuePos1x1SetTo(cellValue);

    EXPECT_THAT(grid, Eq(expectedGrid));

    EXPECT_TRUE(foundCells.m_Queue.empty());
}

TEST_F(TestPossibilitiesRemover, RemoveSetValueFromRelatedCells_IgnoreRelatedCellAlreadySet)
{
    const Value cellValue {3};

    Grid grid {m_GridSize};
    FoundCells foundCells;

    SetupCel1x1RelatedCellsGetter(grid);

    auto cell = grid.m_Cells[1][1];
    cell->SetValue(cellValue);

    const Value otherCellFoundValue {1};
    grid.m_Cells[0][0]->SetValue(otherCellFoundValue);

    MakePossibilitiesRemover()->UpdateGrid(*cell, grid, foundCells);

    auto expectedGrid = CreateExpectedGridWithValuePos1x1SetTo(cellValue);
    expectedGrid.m_Cells[0][0]->SetValue(otherCellFoundValue);

    EXPECT_THAT(grid, Eq(expectedGrid));

    EXPECT_TRUE(foundCells.m_Queue.empty());
}

TEST_F(TestPossibilitiesRemover, RemoveSetValueFromRelatedCells_FindNewCellValue)
{
    const Value cellValue {3};

    Grid grid {m_GridSize};
    FoundCells foundCells;

    SetupCel1x1RelatedCellsGetter(grid);

    auto cell = grid.m_Cells[1][1];
    cell->SetValue(cellValue);

    const Value NewFoundValue {2};
    RemoveAllCellPossibilitiesBut(*grid.m_Cells[1][3], {cellValue, NewFoundValue});

    MakePossibilitiesRemover()->UpdateGrid(*cell, grid, foundCells);

    auto expectedGrid = CreateExpectedGridWithValuePos1x1SetTo(cellValue);
    expectedGrid.m_Cells[1][3]->SetValue(NewFoundValue);

    EXPECT_THAT(grid, Eq(expectedGrid));

    ExpectQueueEqual(foundCells.m_Queue, {grid.m_Cells[1][3]});
}

TEST_F(TestPossibilitiesRemover, RemoveSetValueFromRelatedCells_CellValueNotSet)
{
    Grid grid {m_GridSize};
    FoundCells foundCells;

    SetupCel1x1RelatedCellsGetter(grid);

    auto cell = grid.m_Cells[1][1];

    EXPECT_THROW(MakePossibilitiesRemover()->UpdateGrid(*cell, grid, foundCells), std::exception);
}

} /* namespace test */
} /* namespace sudoku */
