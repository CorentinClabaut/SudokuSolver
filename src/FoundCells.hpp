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

    void Enqueue(SharedCell cell)
    {
        std::lock_guard<std::mutex> l(m_Mutex);
        m_Queue.push(cell);
    }
};

} // namespace sudoku
