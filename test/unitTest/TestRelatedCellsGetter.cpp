#include "RelatedCellsGetter.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <boost/algorithm/cxx11/is_permutation.hpp>

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

    void ExpectIsPermutation(CellsGroup const& lhs, CellsGroup const& rhs)
    {
        EXPECT_THAT(lhs.size(), Eq(rhs.size()));
        EXPECT_TRUE(boost::algorithm::is_permutation(lhs, rhs.begin()));
    }

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

    ExpectIsPermutation(relatedCells, expectedCellsGroup);
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

    ExpectIsPermutation(relatedCells, expectedCellsGroup);
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

    ExpectIsPermutation(relatedCells, expectedCellsGroup);
}

TEST_F(TestRelatedCellsGetter, GetAllRelatedCells)
{
    Grid grid(4);

    auto relatedCells = m_RelatedCellsGetter.GetAllRelatedCells(Position{3, 2}, grid);

    CellsGroup expectedCellsGroup {
        grid.m_Cells[3][0],
        grid.m_Cells[3][1],
        grid.m_Cells[3][3],
        grid.m_Cells[0][2],
        grid.m_Cells[1][2],
        grid.m_Cells[2][2],
        grid.m_Cells[2][3],
    };

    ExpectIsPermutation(relatedCells, expectedCellsGroup);
}


} /* namespace test */
} /* namespace sudoku */
