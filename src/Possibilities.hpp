#pragma once

#include <unordered_set>
#include <bitset>

#include "Value.hpp"
#include "Constants.hpp"

namespace sudoku
{

class Possibilities
{
public:
    using PossibilitiesBitSet = std::bitset<constants::MaxGridSize>;

    Possibilities(int gridSize);

    Possibilities(Possibilities const& possibilities);
    Possibilities& operator=(Possibilities const& possibilities);

    void RemovePossibilities(Possibilities const& possibilities);

    void RemovePossibility(Value const& value);

    void SetValue(Value const& value);

    std::optional<Value> GetValue() const;

    int GetNumberPossibilitiesLeft() const;

    bool OnlyOnePossibilityLeft() const;

    bool operator==(Possibilities const& other) const;

    bool operator==(PossibilitiesBitSet const& possibilities) const;

    Value GetPossibilityLeft() const;

    bool Contains(Value value) const;

private:
    void RemovePossibilitiesImpl(PossibilitiesBitSet const& possibilities);

    bool OnlyOnePossibilityLeftImpl() const;

    PossibilitiesBitSet m_Possibilities;
};

} // namespace sudoku
