#pragma once

#include <memory>
#include <shared_mutex>

#include "Possibilities.hpp"

namespace sudoku
{

struct LockedPossibilities
{
    LockedPossibilities(
            Possibilities const& possibilities,
            std::shared_timed_mutex& possibilitiesMutex);

    const std::shared_lock<std::shared_timed_mutex> m_Lock;
    Possibilities const& m_Possibilities;
};

} // namespace sudoku
