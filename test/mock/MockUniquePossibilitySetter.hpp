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
    MOCK_CONST_METHOD3(SetCellsWithUniquePossibility, void(std::vector<SharedCell>& cells, Grid const& grid, FoundCells& foundCells));
};

} /* namespace test */
} /* namespace sudoku */
