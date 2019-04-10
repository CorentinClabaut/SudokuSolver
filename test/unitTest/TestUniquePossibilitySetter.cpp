#include "UniquePossibilitySetter.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "FoundPositions.hpp"

#include "mock/MockUniquePossibilityFinder.hpp"

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

    void ExpectCellHasUniquePossibility(Position const& position, Grid& grid, Value uniqueValue)
    {
        EXPECT_CALL(*m_UniquePossibilityFinder, FindUniquePossibility(position, Ref(grid))).WillOnce(Return(uniqueValue));
    }

    void ExpectCellHasNoUniquePossibility(Position const& position, Grid& grid)
    {
        EXPECT_CALL(*m_UniquePossibilityFinder, FindUniquePossibility(position, Ref(grid))).WillOnce(Return(std::nullopt));
    }

    void DontTryFindingUniquePossibility(Position const& position, Grid& grid)
    {
        EXPECT_CALL(*m_UniquePossibilityFinder, FindUniquePossibility(position, Ref(grid))).Times(0);
    }

    void ExpectCellSetTo(Cell const& cell, Value expectedValue)
    {
        auto cellValue = cell.GetValue();

        EXPECT_THAT(cellValue, Ne(std::nullopt));
        EXPECT_THAT(*cellValue, Eq(expectedValue));
    }

    void ExpectCellNotSet(Cell const& cell)
    {
        auto cellValue = cell.GetValue();

        EXPECT_THAT(cellValue, Eq(std::nullopt));
    }

    template<typename T>
    void ExpectQueueEqual(std::queue<T>& queue, std::vector<T> const& vector)
    {
        EXPECT_THAT(queue.size(), Eq(vector.size()));

        auto vectorIter = vector.begin();
        while (!queue.empty())
        {
            auto val = queue.front();
            EXPECT_THAT(val, Eq(*vectorIter));
            queue.pop();

            vectorIter++;
        }
    }

    std::unique_ptr<UniquePossibilitySetter> MakeUniquePossibilitySetter()
    {
        return std::make_unique<UniquePossibilitySetterImpl>(
                    std::move(m_UniquePossibilityFinder));
    }

    int m_GridSize = 4;

    std::unique_ptr<MockUniquePossibilityFinder> m_UniquePossibilityFinder = std::make_unique<StrictMock<MockUniquePossibilityFinder>>();
};

TEST_F(TestUniquePossibilitySetter, SkipCellAlreadySet)
{
    Grid grid {m_GridSize};
    FoundPositions foundPositions;

    Position position {0, 1};
    grid.GetCell(position).SetValue(1);

    DontTryFindingUniquePossibility(position, grid);

    MakeUniquePossibilitySetter()->SetIfUniquePossibility(position, grid, foundPositions);

    ExpectQueueEqual(foundPositions.m_Queue, {});
}

TEST_F(TestUniquePossibilitySetter, SetOneCellWithUniquePossibility)
{
    Value uniqueValue {4};

    Grid grid {m_GridSize};
    FoundPositions foundPositions;

    Position position {0, 1};

    ExpectCellHasUniquePossibility(position, grid, uniqueValue);

    MakeUniquePossibilitySetter()->SetIfUniquePossibility(position, grid, foundPositions);

    ExpectCellSetTo(grid.GetCell(position), uniqueValue);

    ExpectQueueEqual(foundPositions.m_Queue, {position});
}

TEST_F(TestUniquePossibilitySetter, SetOneCellWithoutUniquePossibility)
{
    Grid grid(m_GridSize);
    FoundPositions foundPositions;

    Position position {0, 1};

    ExpectCellHasNoUniquePossibility(position, grid);

    MakeUniquePossibilitySetter()->SetIfUniquePossibility(position, grid, foundPositions);

    ExpectCellNotSet(grid.GetCell(position));

    ExpectQueueEqual(foundPositions.m_Queue, {});
}

} /* namespace test */
} /* namespace sudoku */
