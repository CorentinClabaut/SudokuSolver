#pragma once

#include "FoundCellsEnqueuer.hpp"
#include <gmock/gmock.h>

namespace sudoku
{
namespace test
{

class MockFoundCellsEnqueuer : public FoundCellsEnqueuer
{
public:
    MOCK_METHOD1(Enqueue, void(SharedCell cell));
};

} /* namespace test */
} /* namespace sudoku */
