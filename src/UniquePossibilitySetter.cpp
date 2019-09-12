#include "UniquePossibilitySetter.hpp"

#include <boost/container/static_vector.hpp>

#include "Grid.hpp"
#include "Constants.hpp"

using namespace sudoku;

namespace
{

using Cells = boost::container::static_vector<std::reference_wrapper<Cell>, MaxGridSize>;

Cells GetAllCells(Range<Position> const& positions, Grid& grid)
{
    Cells cells;

    std::transform(positions.begin(), positions.end(), std::back_inserter(cells), [&grid](auto const& pos){ return std::ref(grid.GetCell(pos)); });

    return cells;
}

PossibilitiesBitSet GetUniquePossibilitiesBitSet(Cells const& cells)
{
    PossibilitiesBitSet seenAtLeastOnce;
    PossibilitiesBitSet seenAtLeastTwiceOrAlreadySet;

    for (auto const& cell : cells)
    {
        auto const& possibilities = cell.get().GetPossibilities();
        auto const& possibilityBitSet = possibilities.GetBitSet();

        if (possibilities.OnlyOnePossibilityLeft())
            seenAtLeastTwiceOrAlreadySet |= possibilityBitSet;
        else
            seenAtLeastTwiceOrAlreadySet |= (seenAtLeastOnce & possibilityBitSet);

        seenAtLeastOnce |= possibilityBitSet;
    }

    return seenAtLeastOnce ^ seenAtLeastTwiceOrAlreadySet;
}

void SetCellIfHasUniquePossibility(Cell& cell, PossibilitiesBitSet const& uniquePossibilitiesBitSet, FoundPositions& foundPositions)
{
    auto const& possibilityBitSet = cell.GetPossibilities().GetBitSet();

    const auto uniquePossibility = Possibilities {possibilityBitSet & uniquePossibilitiesBitSet};

    const auto numberPossibilityLeft = uniquePossibility.Count();

    if (numberPossibilityLeft == 0)
    {
        return;
    }
    else if (numberPossibilityLeft >= 2)
    {
        throw std::runtime_error("Invalid cell with several unique possibilities");
    }

    cell.SetValue(uniquePossibility.GetPossibilityLeft());
    foundPositions.push(cell.GetPosition());
}

void SetUniquePossibilitiesInGroup(Cells& cells, FoundPositions& foundPositions)
{
    const auto uniquePossibilitiesBitSet = GetUniquePossibilitiesBitSet(cells);

    const auto cellsGroupHasUniquePossibility = uniquePossibilitiesBitSet.none();

    if (cellsGroupHasUniquePossibility)
        return;

    for (auto const& cell : cells)
    {
        SetCellIfHasUniquePossibility(cell, uniquePossibilitiesBitSet, foundPositions);
    }
}

} // anonymous namespace

void UniquePossibilitySetterImpl::SetCellsWithUniquePossibility(Grid& grid, FoundPositions& foundPositions) const
{
    const auto groupsPositions = m_RelatedPositionsGetter.GetAllGroupsPositions(grid.GetGridSize());

    for (auto const& positions : groupsPositions)
    {
        auto cells = GetAllCells(positions, grid);

        SetUniquePossibilitiesInGroup(cells, foundPositions);
    }
}
