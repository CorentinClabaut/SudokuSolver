#pragma once

#include "GridStatusGetter.hpp"
#include <gmock/gmock.h>

namespace sudoku
{
namespace test
{

class MockGridStatusGetter : public GridStatusGetter
{
public:
    MOCK_CONST_METHOD1(GetStatus, GridStatus (Grid const& grid));
};

} /* namespace test */
} /* namespace sudoku */
