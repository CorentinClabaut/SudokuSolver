#include "UniquePossibilitySetter.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "FoundCells.hpp"

#include "mock/MockUniquePossibilityFinder.hpp"

#include "Grid.hpp"

using testing::_;
using testing::Ne;
using testing::Eq;
using testing::Ref;
using testing::Return;
using testing::ReturnRef;
using testing::StrictMock;

namespace sudoku
{
namespace test
{

class TestUniquePossibilitySetter : public ::testing::Test
{
public:
    TestUniquePossibilitySetter()
    {}

    void ExpectCellHasUniquePossibility(SharedCell cell, Grid const& grid, Value uniqueValue)
    {
        EXPECT_CALL(*m_UniquePossibilityFinder, FindUniquePossibility(cell->GetPosition(), grid)).WillOnce(Return(uniqueValue));
    }

    void ExpectCellHasNoUniquePossibility(SharedCell cell, Grid const& grid)
    {
        EXPECT_CALL(*m_UniquePossibilityFinder, FindUniquePossibility(cell->GetPosition(), grid)).WillOnce(Return(std::nullopt));
    }

    void DontTryFindingUniquePossibility(SharedCell cell, Grid const& grid)
    {
        EXPECT_CALL(*m_UniquePossibilityFinder, FindUniquePossibility(cell->GetPosition(), grid)).Times(0);
    }

    void ExpectCellSetTo(SharedCell cell, Value expectedValue)
    {
        auto cellValue = cell->GetValue();

        EXPECT_THAT(cellValue, Ne(std::nullopt));
        EXPECT_THAT(*cellValue, Eq(expectedValue));
    }

    void ExpectCellNotSet(SharedCell cell)
    {
        auto cellValue = cell->GetValue();

        EXPECT_THAT(cellValue, Eq(std::nullopt));
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

    std::unique_ptr<UniquePossibilitySetter> MakeUniquePossibilitySetter()
    {
        return std::make_unique<UniquePossibilitySetterImpl>(
                    std::move(m_UniquePossibilityFinder));
    }

    int m_GridSize = 4;

    std::unique_ptr<MockUniquePossibilityFinder> m_UniquePossibilityFinder = std::make_unique<StrictMock<MockUniquePossibilityFinder>>();
};

TEST_F(TestUniquePossibilitySetter, SkipCellAlreadySet)
{
    Grid grid {m_GridSize};
    FoundCells foundCells;

    std::vector<SharedCell> cells = {grid.m_Cells[0][1]};
    grid.m_Cells[0][1]->SetValue(1);

    DontTryFindingUniquePossibility(grid.m_Cells[0][1], grid);

    MakeUniquePossibilitySetter()->SetCellsWithUniquePossibility(cells, grid, foundCells);

    ExpectQueueEqual(foundCells.m_Queue, {});
}

TEST_F(TestUniquePossibilitySetter, SetOneCellWithUniquePossibility)
{
    Value uniqueValue {4};

    Grid grid {m_GridSize};
    FoundCells foundCells;

    std::vector<SharedCell> cells = {grid.m_Cells[0][1]};

    ExpectCellHasUniquePossibility(grid.m_Cells[0][1], grid, uniqueValue);

    MakeUniquePossibilitySetter()->SetCellsWithUniquePossibility(cells, grid, foundCells);

    ExpectCellSetTo(grid.m_Cells[0][1], uniqueValue);

    ExpectQueueEqual(foundCells.m_Queue, {grid.m_Cells[0][1]});
}

TEST_F(TestUniquePossibilitySetter, SetOneCellWithoutUniquePossibility)
{
    Grid grid(m_GridSize);
    FoundCells foundCells;

    std::vector<SharedCell> cells = {grid.m_Cells[0][1]};

    ExpectCellHasNoUniquePossibility(grid.m_Cells[0][1], grid);

    MakeUniquePossibilitySetter()->SetCellsWithUniquePossibility(cells, grid, foundCells);

    ExpectCellNotSet(grid.m_Cells[0][1]);

    ExpectQueueEqual(foundCells.m_Queue, {});
}

TEST_F(TestUniquePossibilitySetter, SetSeveralCellsSetUniquePossibilities)
{
    Value uniqueValue1 {4};
    Value uniqueValue2 {2};

    Grid grid {m_GridSize};
    FoundCells foundCells;

    std::vector<SharedCell> cells = {grid.m_Cells[0][1], grid.m_Cells[0][2], grid.m_Cells[0][3], grid.m_Cells[1][1]};
    grid.m_Cells[1][1]->SetValue(2);

    ExpectCellHasUniquePossibility(grid.m_Cells[0][1], grid, uniqueValue1);
    ExpectCellHasNoUniquePossibility(grid.m_Cells[0][2], grid);
    ExpectCellHasUniquePossibility(grid.m_Cells[0][3], grid, uniqueValue2);
    DontTryFindingUniquePossibility(grid.m_Cells[1][1], grid);

    MakeUniquePossibilitySetter()->SetCellsWithUniquePossibility(cells, grid, foundCells);

    ExpectCellSetTo(grid.m_Cells[0][1], uniqueValue1);
    ExpectCellNotSet(grid.m_Cells[0][2]);
    ExpectCellSetTo(grid.m_Cells[0][3], uniqueValue2);

    ExpectQueueEqual(foundCells.m_Queue, {grid.m_Cells[0][1], grid.m_Cells[0][3]});
}

} /* namespace test */
} /* namespace sudoku */
