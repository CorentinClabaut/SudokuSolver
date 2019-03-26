#include "UniquePossibilityFinder.hpp"

#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext.hpp>
#include <boost/algorithm/string/join.hpp>

#include "RelatedCellsGetter.hpp"
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

void RemoveRelatedCellsPossibilities(Possibilities& possibilities, CellsGroup const& relatedCells)
{
    for(auto const& relatedCell : relatedCells)
    {
        auto const& relatedCellPossibilities = relatedCell->GetPossibilities();

        possibilities.RemovePossibilities(relatedCellPossibilities);

        if (possibilities.GetNumberPossibilitiesLeft() == 0)
            break;
    }
}

std::optional<Value> FindRelatedCellsMissingPossibility(CellsGroup const& relatedCells, int gridSize)
{
    auto possibilities = Possibilities {gridSize};

    RemoveRelatedCellsPossibilities(possibilities, relatedCells);

    if (possibilities.GetNumberPossibilitiesLeft() == 0)
        return {};

    ValidateOnlyOneMissingPossibility(possibilities);

    return possibilities.GetValue();
}
} // anonymous namespace

UniquePossibilityFinderImpl::UniquePossibilityFinderImpl(
        std::unique_ptr<RelatedCellsGetter> relatedCellsGetter) :
    m_RelatedCellsGetter(std::move(relatedCellsGetter))
{}

std::optional<Value> UniquePossibilityFinderImpl::FindUniquePossibility(Position const& cellPosition, Grid const& grid) const
{
    std::array<CellsGroup, 3> allRelatedCells {
        m_RelatedCellsGetter->GetRelatedBlockCells(cellPosition, grid),
        m_RelatedCellsGetter->GetRelatedHorizontalCells(cellPosition, grid),
        m_RelatedCellsGetter->GetRelatedVerticalCells(cellPosition, grid)
    };

    for (const auto& relatedCells : allRelatedCells)
    {
        auto missingPossibility = FindRelatedCellsMissingPossibility(relatedCells, grid.m_GridSize);

        if (missingPossibility)
            return missingPossibility;
    }

    return {};
}
