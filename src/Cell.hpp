#pragma once

#include <memory>
#include <shared_mutex>

#include "PossibleValues.hpp"
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

    Position const& GetPosition() const;

private:
    PossibleValues m_Possibilities;
    mutable std::shared_timed_mutex m_PossibilitiesMutex;

    const Position m_Position;
};

using SharedCell = std::shared_ptr<Cell>;

} // namespace sudoku
