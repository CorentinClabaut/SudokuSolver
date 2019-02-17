#include "Cell.hpp"

#include <sstream>

using namespace sudoku;

Cell::Cell(Position position, int gridSize) :
    m_Possibilities(CreatePossibilities<Possibilities>(gridSize)),
    m_Position(std::move(position))
{}

void Cell::RemovePossibility(Value const& value)
{
    std::unique_lock<std::shared_timed_mutex> writeLock(m_PossibilitiesMutex);

    m_Possibilities.erase(value);

    if (m_Possibilities.empty())
    {
        std::stringstream error;
        error << "Removed last possibility from cell position '" << m_Position << "'";
        throw std::runtime_error(error.str());
    }
}

void Cell::SetValue(Value const& value)
{
    std::unique_lock<std::shared_timed_mutex> writeLock(m_PossibilitiesMutex);

    if (!m_Possibilities.count(value))
    {
        std::stringstream error;
        error << "try to set cell '" << m_Position << "', with impossible value '" << value << "'";
        throw std::runtime_error(error.str());
    }

    m_Possibilities = {value};
}

std::optional<Value> Cell::GetValue() const
{
    std::shared_lock<std::shared_timed_mutex> readLock(m_PossibilitiesMutex);

    if (m_Possibilities.size() != 1)
        return {};

    return *m_Possibilities.begin();
}

Possibilities Cell::GetPossibilities() const
{
    std::shared_lock<std::shared_timed_mutex> readLock(m_PossibilitiesMutex);

    return m_Possibilities;
}

const Position& Cell::GetPosition() const
{
    return m_Position;
}

bool Cell::operator==(Cell const& cell) const
{
    return m_Possibilities == cell.m_Possibilities
            && m_Position == cell.m_Position;
}
