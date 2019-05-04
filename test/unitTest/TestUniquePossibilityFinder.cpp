#include "UniquePossibilityFinder.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "mock/MockRelatedPositionsGetter.hpp"

#include "Grid.hpp"

using testing::Eq;
using testing::Ne;
using testing::Return;

namespace sudoku
{
namespace test
{

class TestUniquePossibilityFinder : public ::testing::Test
{
public:
    TestUniquePossibilityFinder()
    {}

    void SetupCel1x1RelatedPositionsGetter(Grid& grid)
    {
        EXPECT_CALL(*m_RelatedPositionsGetter, GetRelatedHorizontalPositions(Position{1, 1}, grid.GetGridSize()))
                .WillRepeatedly(Return(Range{m_1x1HorizontalRelatedPositions}));

        EXPECT_CALL(*m_RelatedPositionsGetter, GetRelatedVerticalPositions(Position{1, 1}, grid.GetGridSize()))
                .WillRepeatedly(Return(Range{m_1x1VerticalRelatedPositions}));

        EXPECT_CALL(*m_RelatedPositionsGetter, GetRelatedBlockPositions(Position{1, 1}, grid.GetGridSize()))
                .WillRepeatedly(Return(Range{m_1x1BlockRelatedPositions}));
    }

    std::unique_ptr<UniquePossibilityFinder> MakeUniquePossibilityFinder()
    {
        return std::make_unique<UniquePossibilityFinderImpl>(std::move(m_RelatedPositionsGetter));
    }

    const int m_GridSize {4};

    const std::array<Position, 3> m_1x1HorizontalRelatedPositions {Position{1, 0}, Position{1, 2}, Position{1, 3}};
    const std::array<Position, 3> m_1x1VerticalRelatedPositions {Position{0, 1}, Position{2, 1}, Position{3, 1}};
    const std::array<Position, 3> m_1x1BlockRelatedPositions {Position{0, 0}, Position{0, 1}, Position{1, 0}};

    std::unique_ptr<MockRelatedPositionsGetter> m_RelatedPositionsGetter = std::make_unique<MockRelatedPositionsGetter>();
};

TEST_F(TestUniquePossibilityFinder, HasUniquePossibilityHorizontally)
{
    const Value uniqueValue {1};

    Grid grid {m_GridSize};
    grid.GetCell(Position{1, 0}).RemovePossibility(uniqueValue);
    grid.GetCell(Position{1, 2}).RemovePossibility(uniqueValue);
    grid.GetCell(Position{1, 3}).RemovePossibility(uniqueValue);

    SetupCel1x1RelatedPositionsGetter(grid);

    auto valueFound = MakeUniquePossibilityFinder()->FindUniquePossibility(grid.GetCell(Position{1, 1}), grid);

    EXPECT_THAT(valueFound, Ne(std::nullopt));
    EXPECT_THAT(*valueFound, Eq(uniqueValue));
}

TEST_F(TestUniquePossibilityFinder, HasUniquePossibilityVertically)
{
    const Value uniqueValue {1};

    Grid grid {m_GridSize};
    grid.GetCell(Position{0, 1}).RemovePossibility(uniqueValue);
    grid.GetCell(Position{2, 1}).RemovePossibility(uniqueValue);
    grid.GetCell(Position{3, 1}).RemovePossibility(uniqueValue);

    SetupCel1x1RelatedPositionsGetter(grid);

    auto valueFound = MakeUniquePossibilityFinder()->FindUniquePossibility(grid.GetCell(Position{1, 1}), grid);

    EXPECT_THAT(valueFound, Ne(std::nullopt));
    EXPECT_THAT(*valueFound, Eq(uniqueValue));
}

TEST_F(TestUniquePossibilityFinder, HasUniquePossibilityInBlock)
{
    const Value uniqueValue {1};

    Grid grid {m_GridSize};
    grid.GetCell(Position{0, 0}).RemovePossibility(uniqueValue);
    grid.GetCell(Position{0, 1}).RemovePossibility(uniqueValue);
    grid.GetCell(Position{1, 0}).RemovePossibility(uniqueValue);

    SetupCel1x1RelatedPositionsGetter(grid);

    auto valueFound = MakeUniquePossibilityFinder()->FindUniquePossibility(grid.GetCell(Position{1, 1}), grid);

    EXPECT_THAT(valueFound, Ne(std::nullopt));
    EXPECT_THAT(*valueFound, Eq(uniqueValue));
}

TEST_F(TestUniquePossibilityFinder, HasNoUniquePossibilityInBlock)
{
    const Value uniqueValue {1};

    Grid grid {m_GridSize};
    grid.GetCell(Position{1, 0}).RemovePossibility(uniqueValue);
    grid.GetCell(Position{1, 2}).RemovePossibility(uniqueValue);
    grid.GetCell(Position{0, 1}).RemovePossibility(uniqueValue);
    grid.GetCell(Position{2, 1}).RemovePossibility(uniqueValue);
    grid.GetCell(Position{0, 1}).RemovePossibility(uniqueValue);
    grid.GetCell(Position{1, 0}).RemovePossibility(uniqueValue);

    SetupCel1x1RelatedPositionsGetter(grid);

    auto valueFound = MakeUniquePossibilityFinder()->FindUniquePossibility(grid.GetCell(Position{1, 1}), grid);

    EXPECT_THAT(valueFound, Eq(std::nullopt));
}

} /* namespace test */
} /* namespace sudoku */
