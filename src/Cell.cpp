#include "Cell.hpp"

using namespace sudoku;

Cell::Cell(Position position, int gridSize) :
    m_Possibilities(gridSize),
    m_Position(std::move(position))
{}

Cell::Cell(Cell const& cell) :
    m_Possibilities(cell.m_Possibilities),
    m_Position(cell.m_Position)
{}

Cell& Cell::operator=(Cell const& cell)
{
    m_Possibilities = cell.m_Possibilities;

    return *this;
}

void Cell::RemovePossibility(Value const& value)
{
    m_Possibilities.RemovePossibility(value);

    if (m_Possibilities.Count() == 0)
        throw std::runtime_error("Removed last possibility from cell");
}

void Cell::SetValue(Value const& value)
{
    m_Possibilities.SetValue(value);
}

std::optional<Value> Cell::GetValue() const
{
    return m_Possibilities.GetValue();
}

bool Cell::IsSet() const
{
    return m_Possibilities.OnlyOnePossibilityLeft();
}

Possibilities const& Cell::GetPossibilities() const
{
    return m_Possibilities;
}

int Cell::GetNumberPossibilitiesLeft() const
{
    return m_Possibilities.Count();
}

Position const& Cell::GetPosition() const
{
    return m_Position;
}

bool Cell::operator==(Cell const& cell) const
{
    return m_Possibilities == cell.m_Possibilities
            && m_Position == cell.m_Position;
}
