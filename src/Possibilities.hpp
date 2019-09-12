#pragma once

#include <unordered_set>
#include <bitset>

#include "Value.hpp"
#include "Constants.hpp"

namespace sudoku
{

using PossibilitiesBitSet = std::bitset<MaxGridSize>;

class Possibilities
{
public:
    Possibilities(int gridSize);
    Possibilities(PossibilitiesBitSet const& possibilities);

    Possibilities(Possibilities const& possibilities) = default;
    Possibilities& operator=(Possibilities const& possibilities) = default;

    void RemovePossibility(Value const& value);

    void SetValue(Value const& value);

    std::optional<Value> GetValue() const;

    int Count() const;

    bool OnlyOnePossibilityLeft() const;

    bool operator==(Possibilities const& other) const;

    bool operator==(PossibilitiesBitSet const& possibilities) const;

    Value GetPossibilityLeft() const;

    bool Contains(Value value) const;

    PossibilitiesBitSet const& GetBitSet() const;

private:
    bool OnlyOnePossibilityLeftImpl() const;

    PossibilitiesBitSet m_Possibilities;
};

} // namespace sudoku
