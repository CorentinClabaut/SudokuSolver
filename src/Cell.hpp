#pragma once

#include <memory>
#include <shared_mutex>

#include "Possibilities.hpp"
#include "Position.hpp"
#include "LockedPossibilites.hpp"

namespace sudoku
{

class Cell
{
public:
    Cell(Position position, int gridSize);
    Cell(Cell const& cell);

    Cell& operator=(Cell const& cell);

    void RemovePossibility(Value const& value);

    void SetValue(Value const& value);
    std::optional<Value> GetValue() const;

    bool IsSet() const;
    LockedPossibilities GetLockedPossibilities() const;
    int GetNumberPossibilitiesLeft() const;

    Position const& GetPosition() const;

    bool operator==(Cell const& cell) const;

private:
    Possibilities m_Possibilities;
    mutable std::shared_timed_mutex m_PossibilitiesMutex;

    const Position m_Position;
};

using SharedCell = std::shared_ptr<Cell>;

} // namespace sudoku
