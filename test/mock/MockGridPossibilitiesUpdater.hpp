#pragma once

#include "GridPossibilitiesUpdater.hpp"
#include <gmock/gmock.h>

namespace sudoku
{
namespace test
{

class MockGridPossibilitiesUpdater : public GridPossibilitiesUpdater
{
public:
    MOCK_METHOD2(UpdateGrid, void(FoundPositions& foundPositions, Grid& grid));
};

} /* namespace test */
} /* namespace sudoku */
