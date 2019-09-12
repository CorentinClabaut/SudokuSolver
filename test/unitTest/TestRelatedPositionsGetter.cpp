#include "RelatedPositionsGetter.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Grid.hpp"

using testing::Eq;

namespace sudoku
{
namespace test
{

class TestRelatedPositionsGetter : public ::testing::Test
{
public:
    TestRelatedPositionsGetter()
    {}

    template<typename TContainer1, typename TContainer2>
    bool IsPermutation(TContainer1 const& lhs, TContainer2 const& rhs)
    {
        return std::is_permutation(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    RelatedPositionsGetterImpl m_RelatedPositionsGetter;
};

TEST_F(TestRelatedPositionsGetter, GetRelatedHorizontalPositions)
{
    const int gridSize {4};

    auto relatedPositions = m_RelatedPositionsGetter.GetRelatedHorizontalPositions(Position{3, 2}, gridSize);

    std::vector<Position> expectedPositionsGroup {
        Position{3, 0},
        Position{3, 1},
        Position{3, 3},
    };

    EXPECT_TRUE(IsPermutation(relatedPositions, expectedPositionsGroup));
}

TEST_F(TestRelatedPositionsGetter, GetRelatedVerticalPositions)
{
    const int gridSize {4};

    auto relatedPositions = m_RelatedPositionsGetter.GetRelatedVerticalPositions(Position{3, 2}, gridSize);

    std::vector<Position> expectedPositionsGroup {
        Position{0, 2},
        Position{1, 2},
        Position{2, 2},
    };

    EXPECT_TRUE(IsPermutation(relatedPositions, expectedPositionsGroup));
}

TEST_F(TestRelatedPositionsGetter, GetRelatedBlockPositions)
{
    const int gridSize {4};

    auto relatedPositions = m_RelatedPositionsGetter.GetRelatedBlockPositions(Position{3, 2}, gridSize);

    std::vector<Position> expectedPositionsGroup {
        Position{2, 2},
        Position{2, 3},
        Position{3, 3},
    };

    EXPECT_TRUE(IsPermutation(relatedPositions, expectedPositionsGroup));
}

TEST_F(TestRelatedPositionsGetter, GetAllRelatedPositions)
{
    const int gridSize {4};

    auto relatedPositions = m_RelatedPositionsGetter.GetAllRelatedPositions(Position{3, 2}, gridSize);

    std::vector<Position> expectedPositionsGroup {
        Position{3, 0},
        Position{3, 1},
        Position{3, 3},
        Position{0, 2},
        Position{1, 2},
        Position{2, 2},
        Position{2, 3},
    };

    EXPECT_TRUE(IsPermutation(relatedPositions, expectedPositionsGroup));
}

TEST_F(TestRelatedPositionsGetter, GetAllGroupsPositions)
{
    const int gridSize {4};

    auto relatedPositions = m_RelatedPositionsGetter.GetAllGroupsPositions(gridSize);

    EXPECT_THAT(relatedPositions.size(), Eq(12));

    std::vector<Position> expectedHorizontal {
        Position{3, 0},
        Position{3, 1},
        Position{3, 2},
        Position{3, 3},
    };

    std::vector<Position> expectedVertical {
        Position{0, 2},
        Position{1, 2},
        Position{2, 2},
        Position{3, 2},
    };

    std::vector<Position> expectedBlock {
        Position{2, 2},
        Position{2, 3},
        Position{3, 2},
        Position{3, 3},
    };

    for (auto const& expected : {expectedHorizontal, expectedVertical, expectedBlock})
    {
        EXPECT_THAT(
                std::count_if(relatedPositions.begin(), relatedPositions.end(), [this, expected](auto const& positions){ return IsPermutation(positions, expected); }),
                Eq(1));
    }
}

} /* namespace test */
} /* namespace sudoku */
