#pragma once

#include <queue>
#include <mutex>

#include "Cell.hpp"

namespace sudoku
{

struct FoundCells
{
    std::queue<SharedCell> m_FoundCells;
    std::mutex m_Mutex;
};

} // namespace sudoku
