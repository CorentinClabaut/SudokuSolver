#pragma once

#include "UniquePossibilitySetter.hpp"
#include <gmock/gmock.h>

namespace sudoku
{
namespace test
{

class MockUniquePossibilitySetter : public UniquePossibilitySetter
{
public:
    MOCK_CONST_METHOD3(SetIfUniquePossibility, void(Position const& position, Grid& grid, FoundPositions& foundPositions));
};

} /* namespace test */
} /* namespace sudoku */
