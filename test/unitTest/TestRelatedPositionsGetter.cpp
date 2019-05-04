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
    void ExpectIsPermutation(TContainer1 const& lhs, TContainer2 const& rhs)
    {
        EXPECT_THAT(lhs.size(), Eq(rhs.size()));
        EXPECT_TRUE(std::is_permutation(lhs.begin(), lhs.end(), rhs.begin()));
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

    ExpectIsPermutation(relatedPositions, expectedPositionsGroup);
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

    ExpectIsPermutation(relatedPositions, expectedPositionsGroup);
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

    ExpectIsPermutation(relatedPositions, expectedPositionsGroup);
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

    ExpectIsPermutation(relatedPositions, expectedPositionsGroup);
}

} /* namespace test */
} /* namespace sudoku */
