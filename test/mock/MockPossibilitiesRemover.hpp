#pragma once

#include "PossibilitiesRemover.hpp"
#include <gmock/gmock.h>

namespace sudoku
{
namespace test
{

class MockPossibilitiesRemover : public PossibilitiesRemover
{
public:
    MOCK_CONST_METHOD3(UpdateGrid, void(Cell const& newFoundCell, Grid& grid, FoundCellsEnqueuer& foundCellsEnqueuer));
};

} /* namespace test */
} /* namespace sudoku */
