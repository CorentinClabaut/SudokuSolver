#include "Possibilities.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using testing::Eq;
using testing::Ne;

namespace sudoku
{
namespace test
{

class TestPossibilities : public ::testing::Test
{
public:
    TestPossibilities()
    {}
};

TEST_F(TestPossibilities, AllPossibilitiesSetAtConstruction)
{
    EXPECT_THAT(Possibilities {4}, Eq(PossibilitiesBitSet{0b1111}));

    EXPECT_THAT(Possibilities {9}, Eq(PossibilitiesBitSet{0b111111111}));
}

TEST_F(TestPossibilities, RemoveOneValue)
{
    Possibilities possibilities {4};

    possibilities.RemovePossibility(3);

    EXPECT_THAT(possibilities, Eq(PossibilitiesBitSet{0b1011}));
}

TEST_F(TestPossibilities, RemoveSameValueTwice)
{
    Possibilities possibilities {4};

    possibilities.RemovePossibility(3);
    possibilities.RemovePossibility(3);

    EXPECT_THAT(possibilities, Eq(PossibilitiesBitSet{0b1011}));
}

TEST_F(TestPossibilities, CopyConstructorCopyCorrectPossibilities)
{
    Possibilities possibilities {4};
    possibilities.RemovePossibility(2);

    Possibilities copy {possibilities};

    EXPECT_THAT(possibilities, Eq(copy));
    EXPECT_THAT(copy, Eq(PossibilitiesBitSet{0b1101}));
}

TEST_F(TestPossibilities, SetValue)
{
    Possibilities possibilities {4};

    possibilities.SetValue(3);

    EXPECT_THAT(possibilities, Eq(PossibilitiesBitSet{0b0100}));
}

TEST_F(TestPossibilities, SetSameValueTwice)
{
    Possibilities possibilities {4};

    possibilities.SetValue(3);
    possibilities.SetValue(3);

    EXPECT_THAT(possibilities, Eq(PossibilitiesBitSet{0b0100}));
}

TEST_F(TestPossibilities, SetRemovedValueThrow)
{
    Possibilities possibilities {4};
    possibilities.RemovePossibility(3);

    EXPECT_THROW(possibilities.SetValue(3), std::exception);
}

TEST_F(TestPossibilities, GetValueReturnNoneIfSeveralPossibilitiesLeft)
{
    Possibilities possibilities {4};

    auto value = possibilities.GetValue();

    EXPECT_FALSE(value.has_value());
}

TEST_F(TestPossibilities, GetValueWhenOnlyOnePossibilityLeft)
{
    Possibilities possibilities {4};

    possibilities.RemovePossibility(2);
    possibilities.RemovePossibility(4);
    possibilities.RemovePossibility(1);

    auto value = possibilities.GetValue();

    EXPECT_TRUE(value.has_value());
    EXPECT_THAT(*value, Eq(3));
}

TEST_F(TestPossibilities, GetPossibilityLeft)
{
    Possibilities possibilities {4};

    possibilities.RemovePossibility(2);
    possibilities.RemovePossibility(4);

    auto value = possibilities.GetPossibilityLeft();

    EXPECT_TRUE(value == 1 || value == 3);
}

TEST_F(TestPossibilities, GetNumberPossibilitieLeftAndOnlyOnePossibilityLeft)
{
    Possibilities possibilities {4};
    EXPECT_THAT(possibilities.Count(), Eq(4));
    EXPECT_FALSE(possibilities.OnlyOnePossibilityLeft());

    possibilities.RemovePossibility(2);
    EXPECT_THAT(possibilities.Count(), Eq(3));
    EXPECT_FALSE(possibilities.OnlyOnePossibilityLeft());

    possibilities.RemovePossibility(4);
    EXPECT_THAT(possibilities.Count(), Eq(2));
    EXPECT_FALSE(possibilities.OnlyOnePossibilityLeft());

    possibilities.RemovePossibility(1);
    EXPECT_THAT(possibilities.Count(), Eq(1));
    EXPECT_TRUE(possibilities.OnlyOnePossibilityLeft());
}

} // namespace test
} // namespace sudoku
