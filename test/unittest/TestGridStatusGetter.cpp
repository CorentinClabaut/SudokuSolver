#include "GridStatusGetter.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <boost/range/algorithm/transform.hpp>

#include "Grid.hpp"
#include "GridStatus.hpp"
#include "mock/MockRelatedCellsGetter.hpp"
#include "utils/Utils.hpp"

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

/*
Note: This tests depend on RelatedCellsGetter
*/

class TestGridStatusGetter : public ::testing::Test
{
public:
    TestGridStatusGetter()
    {}

    std::unique_ptr<GridStatusGetter> MakeGridStatusGetter()
    {
        return std::make_unique<GridStatusGetterImpl>(std::move(m_RelatedCellsGetter));
    }

    std::unique_ptr<RelatedCellsGetter> m_RelatedCellsGetter = std::make_unique<RelatedCellsGetterImpl>();
};

TEST_F(TestGridStatusGetter, NoCellsSet)
{
    Grid grid(4);

    EXPECT_THAT(MakeGridStatusGetter()->GetStatus(grid), Eq(GridStatus::Incomplete));
}

TEST_F(TestGridStatusGetter, SomeCellsSet_Valid)
{
    Grid grid(4);

    grid.m_Cells[0][0]->SetValue(1);
    grid.m_Cells[0][3]->SetValue(4);
    grid.m_Cells[1][1]->SetValue(4);
    grid.m_Cells[3][0]->SetValue(2);

    EXPECT_THAT(MakeGridStatusGetter()->GetStatus(grid), Eq(GridStatus::Incomplete));
}

TEST_F(TestGridStatusGetter, SomeCellsSet_TwiceSameValueOnHorizontalLine)
{
    Grid grid(4);

    grid.m_Cells[0][0]->SetValue(1);
    grid.m_Cells[0][3]->SetValue(1);
    grid.m_Cells[1][1]->SetValue(4);
    grid.m_Cells[3][0]->SetValue(2);

    EXPECT_THAT(MakeGridStatusGetter()->GetStatus(grid), Eq(GridStatus::Wrong));
}

TEST_F(TestGridStatusGetter, SomeCellsSet_TwiceSameValueOnVerticalLine)
{
    Grid grid(4);

    grid.m_Cells[0][0]->SetValue(1);
    grid.m_Cells[0][3]->SetValue(4);
    grid.m_Cells[1][1]->SetValue(4);
    grid.m_Cells[3][0]->SetValue(1);

    EXPECT_THAT(MakeGridStatusGetter()->GetStatus(grid), Eq(GridStatus::Wrong));
}

TEST_F(TestGridStatusGetter, SomeCellsSet_TwiceSameValueInBlock)
{
    Grid grid(4);

    grid.m_Cells[0][0]->SetValue(1);
    grid.m_Cells[0][3]->SetValue(4);
    grid.m_Cells[1][1]->SetValue(1);
    grid.m_Cells[3][0]->SetValue(2);

    EXPECT_THAT(MakeGridStatusGetter()->GetStatus(grid), Eq(GridStatus::Wrong));
}

TEST_F(TestGridStatusGetter, AllCellsSet_Valid)
{
    Grid grid = Create4x4CorrectlySolvedGrid();

    EXPECT_THAT(MakeGridStatusGetter()->GetStatus(grid), Eq(GridStatus::SolvedCorrectly));
}

TEST_F(TestGridStatusGetter, AllCellsSet_Invalid)
{
    Grid grid = Create4x4IncorrectlySolvedGrid();

    EXPECT_THAT(MakeGridStatusGetter()->GetStatus(grid), Eq(GridStatus::Wrong));
}

} /* namespace test */
} /* namespace sudoku */
