#include "UniquePossibilitySetter.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "mock/MockUniquePossibilityFinder.hpp"
#include "mock/MockFoundCellsEnqueuer.hpp"

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

    void ExpectFoundCellEnqueued(SharedCell cell)
    {
        EXPECT_CALL(*m_FoundCellsEnqueuer, Enqueue(cell)).Times(1);
    }

    void ExpectNoFoundCellEnqueued()
    {
        EXPECT_CALL(*m_FoundCellsEnqueuer, Enqueue(_)).Times(0);
    }

    void ExpectCellHasUniquePossibility(SharedCell cell, Grid const& grid, Value uniqueValue)
    {
        EXPECT_CALL(*m_UniquePossibilityFinder, FindUniquePossibility(cell->GetPosition(), grid)).WillOnce(Return(uniqueValue));
    }

    void ExpectCellHasNoUniquePossibility(SharedCell cell, Grid const& grid)
    {
        EXPECT_CALL(*m_UniquePossibilityFinder, FindUniquePossibility(cell->GetPosition(), grid)).WillOnce(Return(std::nullopt));
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

    std::unique_ptr<UniquePossibilitySetter> MakeUniquePossibilitySetter()
    {
        return std::make_unique<UniquePossibilitySetterImpl>(
                    std::move(m_UniquePossibilityFinder),
                    m_FoundCellsEnqueuer);
    }

    int m_GridSize = 4;

    std::unique_ptr<MockUniquePossibilityFinder> m_UniquePossibilityFinder = std::make_unique<StrictMock<MockUniquePossibilityFinder>>();
    std::shared_ptr<MockFoundCellsEnqueuer> m_FoundCellsEnqueuer = std::make_shared<StrictMock<MockFoundCellsEnqueuer>>();
};

TEST_F(TestUniquePossibilitySetter, SetOneCellWithUniquePossibility)
{
    Value uniqueValue {4};

    Grid grid {m_GridSize};

    std::vector<SharedCell> cells = {grid.m_Cells[0][1]};

    ExpectCellHasUniquePossibility(grid.m_Cells[0][1], grid, uniqueValue);
    ExpectFoundCellEnqueued(grid.m_Cells[0][1]);

    MakeUniquePossibilitySetter()->SetCellsWithUniquePossibility(cells, grid);

    ExpectCellSetTo(grid.m_Cells[0][1], uniqueValue);
}

TEST_F(TestUniquePossibilitySetter, SetOneCellWithoutUniquePossibility)
{
    Grid grid(m_GridSize);

    std::vector<SharedCell> cells = {grid.m_Cells[0][1]};

    ExpectCellHasNoUniquePossibility(grid.m_Cells[0][1], grid);
    ExpectNoFoundCellEnqueued();

    MakeUniquePossibilitySetter()->SetCellsWithUniquePossibility(cells, grid);

    ExpectCellNotSet(grid.m_Cells[0][1]);
}

TEST_F(TestUniquePossibilitySetter, SetSeveralCellsSetUniquePossibilities)
{
    Value uniqueValue1 {4};
    Value uniqueValue2 {2};

    Grid grid {m_GridSize};

    std::vector<SharedCell> cells = {grid.m_Cells[0][1], grid.m_Cells[0][2], grid.m_Cells[0][3]};

    ExpectCellHasUniquePossibility(grid.m_Cells[0][1], grid, uniqueValue1);
    ExpectCellHasNoUniquePossibility(grid.m_Cells[0][2], grid);
    ExpectCellHasUniquePossibility(grid.m_Cells[0][3], grid, uniqueValue2);

    ExpectFoundCellEnqueued(grid.m_Cells[0][1]);
    ExpectFoundCellEnqueued(grid.m_Cells[0][3]);

    MakeUniquePossibilitySetter()->SetCellsWithUniquePossibility(cells, grid);

    ExpectCellSetTo(grid.m_Cells[0][1], uniqueValue1);
    ExpectCellNotSet(grid.m_Cells[0][2]);
    ExpectCellSetTo(grid.m_Cells[0][3], uniqueValue2);
}

} /* namespace test */
} /* namespace sudoku */
