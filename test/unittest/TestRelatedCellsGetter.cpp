#include "RelatedCellsGetter.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Grid.hpp"

using testing::Eq;

namespace sudoku
{
namespace test
{

class TestRelatedCellsGetter : public ::testing::Test
{
public:
    TestRelatedCellsGetter()
    {}

    RelatedCellsGetterImpl m_RelatedCellsGetter;
};

TEST_F(TestRelatedCellsGetter, GetRelatedHorizontalCells)
{
    Grid grid(4);

    auto relatedCells = m_RelatedCellsGetter.GetRelatedHorizontalCells(Position{3, 2}, grid);

    CellsGroup expectedCellsGroup {
        grid.m_Cells[3][0],
        grid.m_Cells[3][1],
        grid.m_Cells[3][3],
    };

    EXPECT_THAT(relatedCells, Eq(expectedCellsGroup));
}

TEST_F(TestRelatedCellsGetter, GetRelatedVerticalCells)
{
    Grid grid(4);

    auto relatedCells = m_RelatedCellsGetter.GetRelatedVerticalCells(Position{3, 2}, grid);

    CellsGroup expectedCellsGroup {
        grid.m_Cells[0][2],
        grid.m_Cells[1][2],
        grid.m_Cells[2][2],
    };

    EXPECT_THAT(relatedCells, Eq(expectedCellsGroup));
}

TEST_F(TestRelatedCellsGetter, GetRelatedBlockCells)
{
    Grid grid(4);

    auto relatedCells = m_RelatedCellsGetter.GetRelatedBlockCells(Position{3, 2}, grid);

    CellsGroup expectedCellsGroup {
        grid.m_Cells[2][2],
        grid.m_Cells[2][3],
        grid.m_Cells[3][3],
    };

    EXPECT_THAT(relatedCells, Eq(expectedCellsGroup));
}


} /* namespace test */
} /* namespace sudoku */
