#include "LockedPossibilites.hpp"

using namespace sudoku;

LockedPossibilities::LockedPossibilities(
        Possibilities const& possibilities,
        std::shared_timed_mutex& possibilitiesMutex) :
    m_Lock(possibilitiesMutex),
    m_Possibilities(possibilities)
{}
