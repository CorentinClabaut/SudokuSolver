#pragma once

#include <memory>
#include <shared_mutex>

#include "Possibilities.hpp"
#include "Position.hpp"

namespace sudoku
{

class Cell
{
public:
    Cell(Position position, int gridSize);

    void RemovePossibility(Value const& value);

    void SetValue(Value const& value);
    std::optional<Value> GetValue() const;

    Possibilities GetPossibilities() const;

    Position const& GetPosition() const;

    bool operator==(Cell const& cell) const;

private:
    Possibilities m_Possibilities;
    mutable std::shared_timed_mutex m_PossibilitiesMutex;

    const Position m_Position;
};

using SharedCell = std::shared_ptr<Cell>;

} // namespace sudoku
