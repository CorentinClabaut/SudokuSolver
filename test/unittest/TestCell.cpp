#include "Cell.hpp"

#include <thread>
#include <atomic>
#include <list>
#include <numeric>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <boost/range/irange.hpp>

#include "utils/Utils.hpp"

using testing::Eq;
using testing::Ne;

namespace sudoku
{
namespace test
{

void GetValueUntilFound(Cell const& cell)
{
    std::optional<Value> value {};

    while(!value)
    {
        value = cell.GetValue();
    }
}

void GetPossibilitiesUntilFound(Cell const& cell)
{
   Possibilities possibilities {};

    while(possibilities.size() != 1)
    {
        possibilities = cell.GetPossibilities();
    }
}

class TestCell : public ::testing::Test
{
public:
    TestCell()
    {}
};

TEST_F(TestCell, CheckPosition)
{
    Position expectedPosition {1, 2};

    Cell cell(expectedPosition, 9);

    EXPECT_THAT(cell.GetPosition(), Eq(expectedPosition));
}

TEST_F(TestCell, ValueNotSet)
{
    Cell cell(Position{1, 2}, 9);

    auto value = cell.GetValue();

    EXPECT_THAT(value, Eq(std::nullopt));
}

TEST_F(TestCell, SetValue)
{
    Value expectedValue = 5;

    Cell cell(Position{1, 2}, 9);

    cell.SetValue(expectedValue);

    auto value = cell.GetValue();

    EXPECT_THAT(value, Ne(std::nullopt));
    EXPECT_THAT(*value, Eq(expectedValue));
}

TEST_F(TestCell, GetPossibilities)
{
    Cell cell(Position{1, 2}, 4);

    auto possibilities = cell.GetPossibilities();

    EXPECT_THAT(possibilities, Eq(Possibilities{1, 2, 3, 4}));

    cell.RemovePossibility(2);

    possibilities = cell.GetPossibilities();

    EXPECT_THAT(possibilities, Eq(Possibilities{1, 3, 4}));
}

TEST_F(TestCell, SetSameValueTwice)
{
    Value expectedValue = 5;

    Cell cell(Position{1, 2}, 9);

    cell.SetValue(expectedValue);
    cell.SetValue(expectedValue);

    auto value = cell.GetValue();

    EXPECT_THAT(value, Ne(std::nullopt));
    EXPECT_THAT(*value, Eq(expectedValue));
}

TEST_F(TestCell, SetImpossibleValue)
{
    Value removedPossibility = 5;

    Cell cell(Position{1, 2}, 9);

    cell.RemovePossibility(removedPossibility);
    EXPECT_THROW(cell.SetValue(removedPossibility), std::exception);
}

TEST_F(TestCell, RemoveAllPossibilitiesButOne)
{
    Value expectedValue = 3;

    Cell cell(Position{1, 2}, 9);

    RemoveAllCellPossibilitiesBut(cell, {expectedValue});

    auto value = cell.GetValue();

    EXPECT_THAT(value, Ne(std::nullopt));
    EXPECT_THAT(*value, Eq(expectedValue));
}

TEST_F(TestCell, RemoveSamePossibilitiesTwice)
{
    Value expectedValue = 3;

    Cell cell(Position{1, 2}, 9);

    RemoveAllCellPossibilitiesBut(cell, {expectedValue});
    RemoveAllCellPossibilitiesBut(cell, {expectedValue});

    auto value = cell.GetValue();

    EXPECT_THAT(value, Ne(std::nullopt));
    EXPECT_THAT(*value, Eq(expectedValue));
}

TEST_F(TestCell, RemoveAllPossibilitiesThrow)
{
    Cell cell(Position{1, 2}, 9);

    EXPECT_THROW(RemoveAllCellPossibilitiesBut(cell, {}), std::exception);
}

TEST_F(TestCell, SeveralThreadsReadAndWriteOnCell)
{
    const int testExecutionCount = 100;
    for([[gnu::unused]] int i : boost::irange(0, testExecutionCount))
    {
        Value expectedValue = 1;

        Cell cell(Position{1, 2}, 9);

        std::thread reader1(GetValueUntilFound, std::cref(cell));
        std::thread reader2(GetPossibilitiesUntilFound, std::cref(cell));

        std::thread writter1(RemoveAllCellPossibilitiesBut, std::ref(cell), std::unordered_set<Value>{expectedValue});
        std::thread writter2(RemoveAllCellPossibilitiesBut, std::ref(cell), std::unordered_set<Value>{expectedValue});

        cell.SetValue(expectedValue);

        writter1.join();
        writter2.join();

        reader1.join();
        reader2.join();

        auto value = cell.GetValue();

        EXPECT_THAT(value, Ne(std::nullopt));
        EXPECT_THAT(*value, Eq(expectedValue));
    }
}

} // namespace test
} // namespace sudoku
