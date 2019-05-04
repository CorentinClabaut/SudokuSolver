#include "UniquePossibilityFinder.hpp"

#include "RelatedPositionsGetter.hpp"
#include "Position.hpp"
#include "Grid.hpp"
#include "Possibilities.hpp"

using namespace sudoku;

namespace
{
void ValidateOnlyOneMissingPossibility(Possibilities const& possibilities)
{
    if (possibilities.GetNumberPossibilitiesLeft() > 1)
    {
        throw std::runtime_error("Detected cell with more than one solution");
    }
}

void ValidatePossibilityValidInCell(Cell const& cell, Value possibility)
{
    const auto poss = cell.GetPossibilities();
    if (!poss.Contains(possibility))
        throw std::runtime_error("Possibility that is invalid in all remaining related cell");
}

void RemoveRelatedCellsPossibilities(Possibilities& possibilities, Range<Position> const& relatedPositions, const Grid& grid)
{
    for(auto const& position : relatedPositions)
    {
        auto const& relatedCellPossibilities = grid.GetCell(position).GetPossibilities();

        possibilities.RemovePossibilities(relatedCellPossibilities);

        if (possibilities.GetNumberPossibilitiesLeft() == 0)
            break;
    }
}

std::optional<Value> FindRelatedCellsMissingPossibility(Cell const& cell, Range<Position> const& relatedPositions, const Grid& grid)
{
    auto possibilities = Possibilities {grid.GetGridSize()};

    RemoveRelatedCellsPossibilities(possibilities, relatedPositions, grid);

    if (possibilities.GetNumberPossibilitiesLeft() == 0)
        return {};

    ValidateOnlyOneMissingPossibility(possibilities);

    const auto possibility = possibilities.GetValue();

    ValidatePossibilityValidInCell(cell, *possibility);

    return possibility;
}
} // anonymous namespace

UniquePossibilityFinderImpl::UniquePossibilityFinderImpl(
        std::unique_ptr<RelatedPositionsGetter> relatedPositionsGetter) :
    m_RelatedPositionsGetter(std::move(relatedPositionsGetter))
{}

std::optional<Value> UniquePossibilityFinderImpl::FindUniquePossibility(Cell const& cell, const Grid& grid) const
{
    const auto& position = cell.GetPosition();

    std::array<Range<Position>, 3> allRelatedPositions {
        m_RelatedPositionsGetter->GetRelatedBlockPositions(position, grid.GetGridSize()),
        m_RelatedPositionsGetter->GetRelatedHorizontalPositions(position, grid.GetGridSize()),
        m_RelatedPositionsGetter->GetRelatedVerticalPositions(position, grid.GetGridSize())
    };

    for (const auto& relatedPositions : allRelatedPositions)
    {
        auto missingPossibility = FindRelatedCellsMissingPossibility(cell, relatedPositions, grid);

        if (missingPossibility)
            return missingPossibility;
    }

    return {};
}
