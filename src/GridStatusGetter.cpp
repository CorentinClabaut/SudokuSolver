#include "GridStatusGetter.hpp"

#include <algorithm>

#include <boost/range/adaptors.hpp>
#include <boost/algorithm/algorithm.hpp>
#include <boost/algorithm/cxx11/all_of.hpp>
#include <boost/algorithm/cxx11/none_of.hpp>

#include "Grid.hpp"
#include "GridStatus.hpp"

using namespace sudoku;

namespace
{
std::vector<std::reference_wrapper<Cell>> GetCellsSet(Grid& grid)
{
    std::vector<std::reference_wrapper<Cell>> cellsSet;

    std::copy_if(grid.begin(), grid.end(), std::back_inserter(cellsSet), [](auto const& cell){ return cell.IsSet(); });

    return cellsSet;
}

std::vector<Value> GetSetCellValues(Range<Position> const& relatedPositions, Grid const& grid)
{
    std::vector<Value> setCells;

    for (auto const& position : relatedPositions)
    {
        auto val = grid.GetCell(position).GetValue();

        if (val)
            setCells.push_back(*val);
    }

    return setCells;
}

bool AreAllCellsSet(Grid& grid)
{
    return std::all_of(grid.begin(), grid.end(), [](auto const& cell){ return cell.IsSet(); });
}
} // anonymous namespace

GridStatus GridStatusGetterImpl::GetStatus(Grid& grid) const
{
    if (!AreSetCellsValid(grid))
        return GridStatus::Wrong;

    return AreAllCellsSet(grid) ? GridStatus::SolvedCorrectly : GridStatus::Incomplete;
}

bool GridStatusGetterImpl::AreSetCellsValid(Grid& grid) const
{
    auto cellsSet = GetCellsSet(grid);

    return AreCellsValid(cellsSet, grid);
}

std::vector<Value> GridStatusGetterImpl::GetRelatedCellsSetValue(Position const& selectedPosition, Grid& grid) const
{
    auto relatedPositions = m_RelatedPositionsGetter.GetAllRelatedPositions(selectedPosition, grid.GetGridSize());

    return GetSetCellValues(relatedPositions, grid);
}

bool GridStatusGetterImpl::IsCellValueValid(Cell const& cell, Grid& grid) const
{
    auto relatedCellSetValues = GetRelatedCellsSetValue(cell.GetPosition(), grid);

    return boost::algorithm::none_of_equal(relatedCellSetValues, *cell.GetValue());
}

bool GridStatusGetterImpl::AreCellsValid(std::vector<std::reference_wrapper<Cell>> const& cellsSet, Grid& grid) const
{
    auto isCellValueValid = [&](Cell const& cell){ return IsCellValueValid(cell, grid); };

    return boost::algorithm::all_of(cellsSet, isCellValueValid);
}
