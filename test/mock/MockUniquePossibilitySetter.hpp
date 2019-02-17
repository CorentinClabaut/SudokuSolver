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
    MOCK_CONST_METHOD2(SetCellsWithUniquePossibility, void(std::vector<SharedCell>& cells, Grid const& grid));
};

} /* namespace test */
} /* namespace sudoku */
