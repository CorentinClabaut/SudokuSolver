#include "UniquePossibilityFinder.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "mock/MockRelatedCellsGetter.hpp"

using testing::Eq;
using testing::Ne;

namespace sudoku
{
namespace test
{

class TestUniquePossibilityFinder : public ::testing::Test
{
public:
    TestUniquePossibilityFinder()
    {}

    void SetupCel1x1RelatedCellsGetter(Grid& grid)
    {
        m_RelatedCellsGetter->ExpectGetRelatedHorizontalCells(grid, {1, 1}, {{1, 0}, {1, 2}, {1, 3}});
        m_RelatedCellsGetter->ExpectGetRelatedVerticalCells(grid, {1, 1}, {{0, 1}, {2, 1}, {3, 1}});
        m_RelatedCellsGetter->ExpectGetRelatedBlockCells(grid, {1, 1}, {{0, 0}, {0, 1}, {1, 0}});
    }

    std::unique_ptr<UniquePossibilityFinder> MakeUniquePossibilityFinder()
    {
        return std::make_unique<UniquePossibilityFinderImpl>(std::move(m_RelatedCellsGetter));
    }

    const int m_GridSize {4};

    std::unique_ptr<MockRelatedCellsGetter> m_RelatedCellsGetter = std::make_unique<MockRelatedCellsGetter>();
};

TEST_F(TestUniquePossibilityFinder, HasUniquePossibilityHorizontally)
{
    const Value uniqueValue {1};

    Grid grid {m_GridSize};
    grid.m_Cells[1][0]->RemovePossibility(uniqueValue);
    grid.m_Cells[1][2]->RemovePossibility(uniqueValue);
    grid.m_Cells[1][3]->RemovePossibility(uniqueValue);

    SetupCel1x1RelatedCellsGetter(grid);

    auto valueFound = MakeUniquePossibilityFinder()->FindUniquePossibility(Position{1, 1}, grid);

    EXPECT_THAT(valueFound, Ne(std::nullopt));
    EXPECT_THAT(*valueFound, Eq(uniqueValue));
}

TEST_F(TestUniquePossibilityFinder, HasUniquePossibilityVertically)
{
    const Value uniqueValue {1};

    Grid grid {m_GridSize};
    grid.m_Cells[0][1]->RemovePossibility(uniqueValue);
    grid.m_Cells[2][1]->RemovePossibility(uniqueValue);
    grid.m_Cells[3][1]->RemovePossibility(uniqueValue);

    SetupCel1x1RelatedCellsGetter(grid);

    auto valueFound = MakeUniquePossibilityFinder()->FindUniquePossibility(Position{1, 1}, grid);

    EXPECT_THAT(valueFound, Ne(std::nullopt));
    EXPECT_THAT(*valueFound, Eq(uniqueValue));
}

TEST_F(TestUniquePossibilityFinder, HasUniquePossibilityInBlock)
{
    const Value uniqueValue {1};

    Grid grid {m_GridSize};
    grid.m_Cells[0][0]->RemovePossibility(uniqueValue);
    grid.m_Cells[0][1]->RemovePossibility(uniqueValue);
    grid.m_Cells[1][0]->RemovePossibility(uniqueValue);

    SetupCel1x1RelatedCellsGetter(grid);

    auto valueFound = MakeUniquePossibilityFinder()->FindUniquePossibility(Position{1, 1}, grid);

    EXPECT_THAT(valueFound, Ne(std::nullopt));
    EXPECT_THAT(*valueFound, Eq(uniqueValue));
}

TEST_F(TestUniquePossibilityFinder, HasNoUniquePossibilityInBlock)
{
    const Value uniqueValue {1};

    Grid grid {m_GridSize};
    grid.m_Cells[1][0]->RemovePossibility(uniqueValue);
    grid.m_Cells[1][2]->RemovePossibility(uniqueValue);
    grid.m_Cells[0][1]->RemovePossibility(uniqueValue);
    grid.m_Cells[2][1]->RemovePossibility(uniqueValue);
    grid.m_Cells[0][1]->RemovePossibility(uniqueValue);
    grid.m_Cells[1][0]->RemovePossibility(uniqueValue);

    SetupCel1x1RelatedCellsGetter(grid);

    auto valueFound = MakeUniquePossibilityFinder()->FindUniquePossibility(Position{1, 1}, grid);

    EXPECT_THAT(valueFound, Eq(std::nullopt));
}

} /* namespace test */
} /* namespace sudoku */
