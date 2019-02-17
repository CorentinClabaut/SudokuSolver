#pragma once

#include "UniquePossibilityFinder.hpp"
#include <gmock/gmock.h>

namespace sudoku
{
namespace test
{

class MockUniquePossibilityFinder : public UniquePossibilityFinder
{
public:
    MOCK_CONST_METHOD2(FindUniquePossibility, std::optional<Value>(Position const& cellPosition, Grid const& grid));
};

} /* namespace test */
} /* namespace sudoku */
