#include "GridStatusGetter.hpp"

#include <algorithm>

#include <boost/range/adaptors.hpp>
#include <boost/algorithm/algorithm.hpp>
#include <boost/algorithm/cxx11/all_of.hpp>
#include <boost/algorithm/cxx11/none_of.hpp>

#include "Grid.hpp"
#include "RelatedCellsGetter.hpp"
#include "GridStatus.hpp"

using namespace sudoku;

namespace
{
std::vector<SharedCell> GetCellsSet(Grid const& grid)
{
    std::vector<SharedCell> cellsSet;

    std::copy_if(grid.begin(), grid.end(), std::back_inserter(cellsSet), [](auto const& cell){ return cell->IsSet(); });

    return cellsSet;
}

std::vector<Value> GetSetCellValues(std::vector<SharedCell> const& relatedCells)
{
    std::vector<Value> setCells;

    for (auto const& cell : relatedCells)
    {
        auto val = cell->GetValue();

        if (val)
            setCells.push_back(*val);
    }

    return setCells;
}

bool AreAllCellsSet(Grid const& grid)
{
    return std::all_of(grid.begin(), grid.end(), [](auto const& cell){ return cell->IsSet(); });
}
} // anonymous namespace

GridStatusGetterImpl::GridStatusGetterImpl(std::unique_ptr<RelatedCellsGetter> relatedCellsGetter) :
    m_RelatedCellsGetter(std::move(relatedCellsGetter))
{}

GridStatus GridStatusGetterImpl::GetStatus(Grid const& grid) const
{
    if (!AreSetCellsValid(grid))
        return GridStatus::Wrong;

    return AreAllCellsSet(grid) ? GridStatus::SolvedCorrectly : GridStatus::Incomplete;
}

bool GridStatusGetterImpl::AreSetCellsValid(Grid const& grid) const
{
    auto cellsSet = GetCellsSet(grid);

    return AreCellsValid(cellsSet, grid);
}

std::vector<Value> GridStatusGetterImpl::GetRelatedCellsSetValue(Position const& selectedPosition, Grid const& grid) const
{
    auto relatedCells = m_RelatedCellsGetter->GetAllRelatedCells(selectedPosition, grid);

    return GetSetCellValues(relatedCells);
}

bool GridStatusGetterImpl::IsCellValueValid(Cell const& cell, Grid const& grid) const
{
    auto relatedCellSetValues = GetRelatedCellsSetValue(cell.GetPosition(), grid);

    return boost::algorithm::none_of_equal(relatedCellSetValues, *cell.GetValue());
}

bool GridStatusGetterImpl::AreCellsValid(std::vector<SharedCell> const& cellsSet, Grid const& grid) const
{
    auto isCellValueValid = [&](SharedCell const& cell){ return IsCellValueValid(*cell, grid); };

    return boost::algorithm::all_of(cellsSet, isCellValueValid);
}
