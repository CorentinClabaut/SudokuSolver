#pragma once

#include <unordered_set>

#include <boost/range/irange.hpp>

#include "Value.hpp"

namespace sudoku
{

using Possibilities = std::unordered_set<Value>;

template<typename TContainer>
TContainer CreatePossibilities(int gridSize)
{
    return boost::copy_range<TContainer>(boost::irange(1, gridSize + 1));
}

} // namespace sudoku
