#include "ThreadSafePossibilities.hpp"

using namespace sudoku;

ThreadSafePossibilities::ThreadSafePossibilities(int gridSize) :
    m_Possibilities(gridSize)
{}

ThreadSafePossibilities::ThreadSafePossibilities(ThreadSafePossibilities const& possibilities) :
    m_Possibilities(possibilities.m_Possibilities)
{}

ThreadSafePossibilities& ThreadSafePossibilities::operator=(ThreadSafePossibilities const& possibilities)
{
    m_Possibilities = possibilities.m_Possibilities;

    return *this;
}

void ThreadSafePossibilities::RemovePossibility(Value const& value)
{
    std::unique_lock<std::shared_timed_mutex> writeLock(m_PossibilitiesMutex);

    m_Possibilities.RemovePossibility(value);
}

void ThreadSafePossibilities::SetValue(Value const& value)
{
    std::unique_lock<std::shared_timed_mutex> writeLock(m_PossibilitiesMutex);

    m_Possibilities.SetValue(value);
}

std::optional<Value> ThreadSafePossibilities::GetValue() const
{
    std::shared_lock<std::shared_timed_mutex> readLock(m_PossibilitiesMutex);

    return m_Possibilities.GetValue();
}

int ThreadSafePossibilities::GetNumberPossibilitiesLeft() const
{
    std::shared_lock<std::shared_timed_mutex> readLock(m_PossibilitiesMutex);

    return m_Possibilities.GetNumberPossibilitiesLeft();
}

bool ThreadSafePossibilities::OnlyOnePossibilityLeft() const
{
    std::shared_lock<std::shared_timed_mutex> readLock(m_PossibilitiesMutex);

    return m_Possibilities.OnlyOnePossibilityLeft();
}

Possibilities ThreadSafePossibilities::GetPossibilities() const
{
    return m_Possibilities;
}
