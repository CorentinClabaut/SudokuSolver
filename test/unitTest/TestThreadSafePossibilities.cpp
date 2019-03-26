#include "ThreadSafePossibilities.hpp"

#include <thread>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <boost/range/irange.hpp>

using testing::Eq;
using testing::Ne;

namespace sudoku
{
namespace test
{

class TestThreadSafePossibilities : public ::testing::Test
{
public:
    TestThreadSafePossibilities()
    {}
};

void GetValueUntilFound(ThreadSafePossibilities const& possibilities)
{
    std::optional<Value> value {};

    while(!value)
    {
        value = possibilities.GetValue();
    }
}

void GetNumberPossibilitiesLeftUntilFound(ThreadSafePossibilities const& possibilities)
{
   int possibilitiesLeft {9};

    while(possibilitiesLeft != 1)
    {
        possibilitiesLeft = possibilities.GetNumberPossibilitiesLeft();
    }
}

void TestIfOnlyOnePossibilityLeftUntilTrue(ThreadSafePossibilities const& possibilities)
{
    while(!possibilities.OnlyOnePossibilityLeft())
        ;
}

void RemovePossibilities(
        ThreadSafePossibilities& possibilities,
        std::vector<Value> possibilitiesToRemove)
{
    for (auto possibilityToRemove : possibilitiesToRemove)
        possibilities.RemovePossibility(possibilityToRemove);
}

void SetValue(ThreadSafePossibilities& possibilities)
{
    possibilities.SetValue(3);
}

TEST_F(TestThreadSafePossibilities, SeveralThreadsReadAndWritePossibilities)
{
    const int testExecutionCount = 1000;
    for([[gnu::unused]] int i : boost::irange(0, testExecutionCount))
    {
        ThreadSafePossibilities possibilities {9};

        std::thread reader1(GetValueUntilFound, std::cref(possibilities));
        std::thread reader2(GetNumberPossibilitiesLeftUntilFound, std::cref(possibilities));
        std::thread reader3(TestIfOnlyOnePossibilityLeftUntilTrue, std::cref(possibilities));

        std::thread writter1(RemovePossibilities, std::ref(possibilities), std::vector<Value>{2, 4, 7, 9});
        std::thread writter2(RemovePossibilities, std::ref(possibilities), std::vector<Value>{1, 2, 5, 6, 7});
        std::thread writter3(SetValue, std::ref(possibilities));

        writter1.join();
        writter2.join();
        writter3.join();

        reader1.join();
        reader2.join();
        reader3.join();

        auto value = possibilities.GetValue();

        EXPECT_TRUE(value.has_value());
        EXPECT_THAT(*value, Eq(3));
    }
}

} // namespace test
} // namespace sudoku
