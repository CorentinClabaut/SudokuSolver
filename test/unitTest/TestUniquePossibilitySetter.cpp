#include "UniquePossibilitySetter.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <boost/range/irange.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/algorithm/cxx11/is_permutation.hpp>

#include "mock/MockUniquePossibilityFinder.hpp"

#include "Grid.hpp"
#include "FoundPositions.hpp"
#include "utils/Utils.hpp"

using testing::_;
using testing::Eq;
using testing::Ref;
using testing::Return;
using testing::Invoke;
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

    std::unique_ptr<UniquePossibilitySetter> MakeUniquePossibilitySetter()
    {
        return std::make_unique<UniquePossibilitySetterImpl>(
                    std::move(m_UniquePossibilityFinder));
    }

    Grid CreateGridWithOneCellNotSet(Position const& cellNotSetPosition)
    {
        const int gridSize {4};
        Grid grid {gridSize};

        for (auto& cell : grid)
        {
            if (cell.GetPosition() == cellNotSetPosition)
                continue;

            cell.SetValue(3);
        }

        return grid;
    }

    std::unique_ptr<MockUniquePossibilityFinder> m_UniquePossibilityFinder = std::make_unique<StrictMock<MockUniquePossibilityFinder>>();
};

TEST_F(TestUniquePossibilitySetter, SetCellsWithUniquePossibility_AllCellsAlreadySet)
{
    FoundPositions foundPositions;

    auto grid = Create4x4CorrectlySolvedGrid();

    MakeUniquePossibilitySetter()->SetCellsWithUniquePossibility(grid, foundPositions);

    EXPECT_TRUE(QueueToVector(foundPositions).empty());
}

TEST_F(TestUniquePossibilitySetter, SetCellsWithUniquePossibility_OneCellNotSetFoundValue)
{
    FoundPositions foundPositions;

    const Position cellNotSetPosition {1, 2};
    auto grid = CreateGridWithOneCellNotSet(cellNotSetPosition);

    const Value foundValue {2};

    EXPECT_CALL(*m_UniquePossibilityFinder, FindUniquePossibility(Ref(grid.GetCell(cellNotSetPosition)), Ref(grid)))
            .WillOnce(Return(foundValue));

    MakeUniquePossibilitySetter()->SetCellsWithUniquePossibility(grid, foundPositions);

    EXPECT_THAT(QueueToVector(foundPositions), Eq(std::vector<Position>{cellNotSetPosition}));
    EXPECT_THAT(grid.GetCell(cellNotSetPosition).GetValue(), Eq(foundValue));
}

TEST_F(TestUniquePossibilitySetter, SetCellsWithUniquePossibility_OneCellNotSetDidNotFoundValue)
{
    FoundPositions foundPositions;

    const Position cellNotSetPosition {1, 2};
    auto grid = CreateGridWithOneCellNotSet(cellNotSetPosition);

    EXPECT_CALL(*m_UniquePossibilityFinder, FindUniquePossibility(Ref(grid.GetCell(cellNotSetPosition)), Ref(grid)))
            .WillOnce(Return(std::optional<Value>{}));

    MakeUniquePossibilitySetter()->SetCellsWithUniquePossibility(grid, foundPositions);

    EXPECT_TRUE(QueueToVector(foundPositions).empty());
    EXPECT_THAT(grid.GetCell(cellNotSetPosition).GetValue(), Eq(std::optional<Value>{}));
}

} /* namespace test */
} /* namespace sudoku */
