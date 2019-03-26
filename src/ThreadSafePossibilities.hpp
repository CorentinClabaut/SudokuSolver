#pragma once

#include <shared_mutex>

#include "Possibilities.hpp"

namespace sudoku
{

class ThreadSafePossibilities
{
public:
    ThreadSafePossibilities(int gridSize);

    ThreadSafePossibilities(ThreadSafePossibilities const& possibilities);
    ThreadSafePossibilities& operator=(ThreadSafePossibilities const& possibilities);

    void RemovePossibility(Value const& value);

    void SetValue(Value const& value);

    std::optional<Value> GetValue() const;

    int GetNumberPossibilitiesLeft() const;

    bool OnlyOnePossibilityLeft() const;

    Possibilities GetPossibilities() const;

private:
    mutable std::shared_timed_mutex m_PossibilitiesMutex;
    Possibilities m_Possibilities;
};

} // namespace sudoku
