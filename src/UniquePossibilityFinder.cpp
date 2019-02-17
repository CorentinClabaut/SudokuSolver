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
void ValidateOnlyOneMissingPossibility(std::vector<Value> const& possibilities, Position const& cellPosition)
{
    if (possibilities.size() > 1)
    {
        auto toString = [](int i){ return std::to_string(i); };

        std::stringstream error;
        error << "Cell " << cellPosition << " contains more than 1 unique possibility ("
              << boost::algorithm::join(possibilities | boost::adaptors::transformed(toString), ", ") << ")";

        throw std::runtime_error(error.str());
    }
}

void RemoveRelatedCellsPossibilities(std::vector<Value>& possibilities, CellsGroup const& relatedCells)
{
    for(auto const& relatedCell : relatedCells)
    {
        const auto relatedCellPossibilities = relatedCell->GetPossibilities();

        boost::range::remove_erase_if(possibilities, [&](Value v){ return relatedCellPossibilities.count(v); });
    }
}

std::optional<Value> FindRelatedCellsMissingPossibility(CellsGroup const& relatedCells, int gridSize, Position const& cellPosition)
{
    auto possibilities = CreatePossibilities<std::vector<Value>>(gridSize);

    RemoveRelatedCellsPossibilities(possibilities, relatedCells);

    if (possibilities.empty())
        return {};

    ValidateOnlyOneMissingPossibility(possibilities, cellPosition);

    return possibilities.front();
}
} // anonymous namespace

UniquePossibilityFinderImpl::UniquePossibilityFinderImpl(
        std::unique_ptr<RelatedCellsGetter> relatedCellsGetter) :
    m_RelatedCellsGetter(std::move(relatedCellsGetter))
{}

std::optional<Value> UniquePossibilityFinderImpl::FindUniquePossibility(Position const& cellPosition, Grid const& grid) const
{
    std::vector<CellsGroup> allRelatedCells {
        m_RelatedCellsGetter->GetRelatedBlockCells(cellPosition, grid),
        m_RelatedCellsGetter->GetRelatedHorizontalCells(cellPosition, grid),
        m_RelatedCellsGetter->GetRelatedVerticalCells(cellPosition, grid)
    };

    for (const auto& relatedCells : allRelatedCells)
    {
        auto missingPossibility = FindRelatedCellsMissingPossibility(relatedCells, grid.m_GridSize, cellPosition);

        if (missingPossibility)
            return missingPossibility;
    }

    return {};
}
