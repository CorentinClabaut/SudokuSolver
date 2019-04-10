#pragma once

#include <queue>
#include <mutex>

#include "Cell.hpp"

namespace sudoku
{

struct FoundPositions
{
    std::queue<Position> m_Queue;
    std::mutex m_Mutex;

    void Enqueue(Position position)
    {
        std::lock_guard<std::mutex> l(m_Mutex);
        m_Queue.push(std::move(position));
    }
};

} // namespace sudoku
