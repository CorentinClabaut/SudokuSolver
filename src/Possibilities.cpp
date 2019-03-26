#include "Possibilities.hpp"

using namespace sudoku;

Possibilities::Possibilities(int gridSize)
{
    if (gridSize > constants::MaxGridSize)
        throw std::runtime_error("Invalid grid size '" + std::to_string(gridSize) + "' max '" + std::to_string(constants::MaxGridSize) + "'");

    for (int i = 0; i < gridSize; i++)
        m_Possibilities.set(i);
}

Possibilities::Possibilities(Possibilities const& possibilities)
{
    m_Possibilities = possibilities.m_Possibilities;
}

Possibilities& Possibilities::operator=(Possibilities const& possibilities)
{
    m_Possibilities = possibilities.m_Possibilities;

    return *this;
}

void Possibilities::RemovePossibilities(Possibilities const& possibilities)
{
    RemovePossibilitiesImpl(possibilities.m_Possibilities);
}

void Possibilities::RemovePossibility(Value const& value)
{
    RemovePossibilitiesImpl(1 << (value - 1));
}

void Possibilities::SetValue(Value const& value)
{
    PossibilitiesBitSet valueBit = 1 << (value - 1);

    if ((m_Possibilities & valueBit) == 0)
        throw std::runtime_error("Try to set cell with impossible value");

    m_Possibilities = valueBit;
}

std::optional<Value> Possibilities::GetValue() const
{
    if (!OnlyOnePossibilityLeftImpl())
        return {};

    return GetPossibilityLeft();
}

Value Possibilities::GetPossibilityLeft() const
{
    auto possibilities = m_Possibilities;

    int val = 0;
    while (possibilities != 0) {
        possibilities >>= 1;
        val++;
    }

    return val;
}

int Possibilities::GetNumberPossibilitiesLeft() const
{
    return m_Possibilities.count();
}

bool Possibilities::OnlyOnePossibilityLeft() const
{
    return OnlyOnePossibilityLeftImpl();
}

bool Possibilities::operator==(Possibilities const& other) const
{
    return m_Possibilities == other.m_Possibilities;
}

bool Possibilities::operator==(PossibilitiesBitSet const& possibilities) const
{
    return m_Possibilities == possibilities;
}

void Possibilities::RemovePossibilitiesImpl(PossibilitiesBitSet const& possibilities)
{
    m_Possibilities = m_Possibilities & ~possibilities;
}

bool Possibilities::OnlyOnePossibilityLeftImpl() const
{
    return m_Possibilities.count() == 1;
}
