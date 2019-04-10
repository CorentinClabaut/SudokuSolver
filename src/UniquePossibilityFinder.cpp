#include "UniquePossibilityFinder.hpp"

#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext.hpp>
#include <boost/algorithm/string/join.hpp>

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

void RemoveRelatedCellsPossibilities(Possibilities& possibilities, std::vector<Position> const& relatedPositions, Grid& grid)
{
    for(auto const& position : relatedPositions)
    {
        auto const& relatedCellPossibilities = grid.GetCell(position).GetPossibilities();

        possibilities.RemovePossibilities(relatedCellPossibilities);

        if (possibilities.GetNumberPossibilitiesLeft() == 0)
            break;
    }
}

std::optional<Value> FindRelatedCellsMissingPossibility(std::vector<Position> const& relatedPositions, Grid& grid)
{
    auto possibilities = Possibilities {grid.GetGridSize()};

    RemoveRelatedCellsPossibilities(possibilities, relatedPositions, grid);

    if (possibilities.GetNumberPossibilitiesLeft() == 0)
        return {};

    ValidateOnlyOneMissingPossibility(possibilities);

    return possibilities.GetValue();
}
} // anonymous namespace

UniquePossibilityFinderImpl::UniquePossibilityFinderImpl(
        std::unique_ptr<RelatedPositionsGetter> relatedPositionsGetter) :
    m_RelatedPositionsGetter(std::move(relatedPositionsGetter))
{}

std::optional<Value> UniquePossibilityFinderImpl::FindUniquePossibility(Position const& cellPosition, Grid& grid) const
{
    std::array<std::vector<Position>, 3> allRelatedPositions {
        m_RelatedPositionsGetter->GetRelatedBlockPositions(cellPosition, grid.GetGridSize(), grid.GetBlockSize()),
        m_RelatedPositionsGetter->GetRelatedHorizontalPositions(cellPosition, grid.GetGridSize()),
        m_RelatedPositionsGetter->GetRelatedVerticalPositions(cellPosition, grid.GetGridSize())
    };

    for (const auto& relatedPositions : allRelatedPositions)
    {
        auto missingPossibility = FindRelatedCellsMissingPossibility(relatedPositions, grid);

        if (missingPossibility)
            return missingPossibility;
    }

    return {};
}
