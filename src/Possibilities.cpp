#include "Possibilities.hpp"

using namespace sudoku;

namespace
{

constexpr int Pow(int value, int power)
{
    return power == 0 ? 1 : value * Pow(value, power - 1);
}

constexpr int GetNumBitSet(int value)
{
    int numBitSet = 0;

    for(; value; value &= value - 1)
        ++numBitSet;

    return numBitSet;
}

template<int TNumPossibilities>
constexpr std::array<int, Pow(2, TNumPossibilities)> CreateNumBitSetLookupTable()
{
    std::array<int, Pow(2, TNumPossibilities)> numBitSetLookupTable {};

    for(int i = 0; i < Pow(2, TNumPossibilities); i++)
    {
        numBitSetLookupTable[i] = GetNumBitSet(i);
    }

    return numBitSetLookupTable;
}

constexpr std::array<int, Pow(2, MaxGridSize)> NumBitSetLookupTable {CreateNumBitSetLookupTable<MaxGridSize>()};

} // anonymous namespace

Possibilities::Possibilities(int gridSize)
{
    if (gridSize > MaxGridSize)
        throw std::runtime_error("Invalid grid size '" + std::to_string(gridSize) + "' max '" + std::to_string(MaxGridSize) + "'");

    for (int i = 0; i < gridSize; i++)
        m_Possibilities.set(i);
}

Possibilities::Possibilities(PossibilitiesBitSet const& possibilities) :
    m_Possibilities(possibilities)
{}

void Possibilities::RemovePossibility(Value const& value)
{
    m_Possibilities.reset(value - 1);
}

void Possibilities::SetValue(Value const& value)
{
    if (!Contains(value))
        throw std::runtime_error("Try to set cell with impossible value");

    m_Possibilities.reset();
    m_Possibilities.set(value -1);
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
    while (possibilities != 0)
    {
        possibilities >>= 1;
        val++;
    }

    return val;
}

bool Possibilities::Contains(Value value) const
{
    return m_Possibilities.test(value - 1);
}

PossibilitiesBitSet const& Possibilities::GetBitSet() const
{
    return m_Possibilities;
}

int Possibilities::Count() const
{
    return NumBitSetLookupTable[m_Possibilities.to_ulong()];
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

bool Possibilities::OnlyOnePossibilityLeftImpl() const
{
    return Count() == 1;
}
