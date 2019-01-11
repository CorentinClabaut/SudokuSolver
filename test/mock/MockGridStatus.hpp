#pragma once

#include "GridStatus.hpp"
#include <gmock/gmock.h>

namespace sudoku
{
namespace test
{

class MockGridStatus : public GridStatus
{
public:
    MOCK_CONST_METHOD1(AreAllCellsSet, bool(Grid const& grid));
    MOCK_CONST_METHOD1(AreSetCellsValid, bool(Grid const& grid));
};

} /* namespace test */
} /* namespace sudoku */
