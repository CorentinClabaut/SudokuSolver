#pragma once

#include <queue>
#include <mutex>

#include "Cell.hpp"

namespace sudoku
{

struct FoundCells
{
    std::queue<SharedCell> m_Queue;
    std::mutex m_Mutex;
};

} // namespace sudoku
