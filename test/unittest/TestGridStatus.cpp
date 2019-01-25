#include "GridStatus.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <boost/range/algorithm/transform.hpp>

#include "Grid.hpp"
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

class TestGridStatus : public ::testing::Test
{
public:
    TestGridStatus()
    {}

    std::unique_ptr<GridStatus> MakeGridStatus()
    {
        return std::make_unique<GridStatusImpl>(std::move(m_RelatedCellsGetter));
    }

    void SetupRelatedCellsGetter(Grid& grid)
    {
        m_RelatedCellsGetter->ExpectGetAllRelatedCells(grid, {0, 0}, {{0, 1}, {0, 2}, {0, 3}, {1, 0}, {2, 0}, {3, 0}, {1, 1}});

        m_RelatedCellsGetter->ExpectGetAllRelatedCells(grid, {0, 3}, {{0, 0}, {0, 1}, {0, 2}, {1, 3}, {2, 3}, {3, 3}, {1, 2}});

        m_RelatedCellsGetter->ExpectGetAllRelatedCells(grid, {1, 1}, {{1, 0}, {1, 2}, {1, 3}, {0, 1}, {2, 1}, {3, 1}, {0, 0}});

        m_RelatedCellsGetter->ExpectGetAllRelatedCells(grid, {3, 0}, {{3, 1}, {3, 2}, {3, 3}, {0, 0}, {1, 0}, {2, 0}, {2, 1}});
    }

    std::unique_ptr<MockRelatedCellsGetter> m_RelatedCellsGetter = std::make_unique<StrictMock<MockRelatedCellsGetter>>();
};

TEST_F(TestGridStatus, AreAllCellsSet_NoneSet)
{
    Grid grid(4);

    EXPECT_FALSE(MakeGridStatus()->AreAllCellsSet(grid));
}

TEST_F(TestGridStatus, AreAllCellsSet_SomeSet)
{
    Grid grid(4);

    grid.m_Cells[0][0]->SetValue(3);
    grid.m_Cells[0][1]->SetValue(2);
    grid.m_Cells[1][0]->SetValue(1);

    EXPECT_FALSE(MakeGridStatus()->AreAllCellsSet(grid));
}

TEST_F(TestGridStatus, AreAllCellsSet_AllSet)
{
    Grid grid(4);

    for (auto& cell : grid)
    {
        cell->SetValue(3);
    }

    EXPECT_TRUE(MakeGridStatus()->AreAllCellsSet(grid));
}

TEST_F(TestGridStatus, SetCellsAreNotValid_TwiceSameValueOnHorizontalLine)
{
    Grid grid(4);

    grid.m_Cells[0][0]->SetValue(2);
    grid.m_Cells[0][3]->SetValue(2);

    SetupRelatedCellsGetter(grid);

    EXPECT_FALSE(MakeGridStatus()->AreSetCellsValid(grid));
}

TEST_F(TestGridStatus, SetCellsAreNotValid_TwiceSameValueOnVerticalLine)
{
    Grid grid(4);

    grid.m_Cells[0][0]->SetValue(2);
    grid.m_Cells[3][0]->SetValue(2);

    SetupRelatedCellsGetter(grid);

    EXPECT_FALSE(MakeGridStatus()->AreSetCellsValid(grid));
}

TEST_F(TestGridStatus, SetCellsAreNotValid_TwiceSameValueInBlock)
{
    Grid grid(4);

    grid.m_Cells[0][0]->SetValue(2);
    grid.m_Cells[1][1]->SetValue(2);

    SetupRelatedCellsGetter(grid);

    EXPECT_FALSE(MakeGridStatus()->AreSetCellsValid(grid));
}

TEST_F(TestGridStatus, SetCellsAreValid)
{
    Grid grid(4);

    grid.m_Cells[0][0]->SetValue(1);
    grid.m_Cells[0][3]->SetValue(4);
    grid.m_Cells[1][1]->SetValue(4);
    grid.m_Cells[3][0]->SetValue(2);

    SetupRelatedCellsGetter(grid);

    EXPECT_TRUE(MakeGridStatus()->AreSetCellsValid(grid));
}

} /* namespace test */
} /* namespace sudoku */
