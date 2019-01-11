#include "GridStatus.hpp"

#include <algorithm>

#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext.hpp>
#include <boost/algorithm/algorithm.hpp>
#include <boost/algorithm/cxx11/all_of.hpp>
#include <boost/algorithm/cxx11/none_of.hpp>

#include "Grid.hpp"
#include "RelatedCellsGetter.hpp"

using namespace sudoku;

namespace
{
std::vector<SharedCell> GetCellsSet(Grid const& grid)
{
    std::vector<SharedCell> cellsSet;

    std::copy_if(grid.begin(), grid.end(), std::back_inserter(cellsSet), [](auto const& cell){ return cell->GetValue(); });

    return cellsSet;
}

std::vector<Value> GetSetCellValues(std::vector<SharedCell> const& relatedCells)
{
    auto values = relatedCells
            | boost::adaptors::transformed(+[](SharedCell const& c){ return c->GetValue(); })
            | boost::adaptors::filtered(+[](std::optional<Value> const& o){ return o.has_value(); })
            | boost::adaptors::transformed(+[](std::optional<Value> const& o){ return o.value(); });

    return boost::copy_range<std::vector<Value>>(values);
}
} // anonymous namespace

GridStatusImpl::GridStatusImpl(std::unique_ptr<RelatedCellsGetter> relatedCellsGetter) :
    m_RelatedCellsGetter(std::move(relatedCellsGetter))
{}

bool GridStatusImpl::AreAllCellsSet(Grid const& grid) const
{
    auto isSet = [](SharedCell const& cell){ return cell->GetValue(); };

    return std::all_of(grid.begin(), grid.end(), isSet);
}

bool GridStatusImpl::AreSetCellsValid(Grid const& grid) const
{
    auto cellsSet = GetCellsSet(grid);

    return AreCellsValid(cellsSet, grid);
}

std::vector<SharedCell> GridStatusImpl::GetRelatedCells(Position const& selectedPosition, Grid const& grid) const
{
    std::vector<SharedCell> relatedCells;

    boost::range::push_back(relatedCells, m_RelatedCellsGetter->GetRelatedHorizontalCells(selectedPosition, grid));
    boost::range::push_back(relatedCells, m_RelatedCellsGetter->GetRelatedVerticalCells(selectedPosition, grid));
    boost::range::push_back(relatedCells, m_RelatedCellsGetter->GetRelatedBlockCells(selectedPosition, grid));

    boost::sort(relatedCells);

    return boost::copy_range<std::vector<SharedCell>>( boost::range::unique(relatedCells) );
}

std::vector<Value> GridStatusImpl::GetRelatedCellsSetValue(Position const& selectedPosition, Grid const& grid) const
{
    auto relatedCells = GetRelatedCells(selectedPosition, grid);

    return GetSetCellValues(relatedCells);
}

bool GridStatusImpl::IsCellValueValid(Cell const& cell, Grid const& grid) const
{
    auto relatedCellSetValues = GetRelatedCellsSetValue(cell.GetPosition(), grid);

    return boost::algorithm::none_of_equal(relatedCellSetValues, *cell.GetValue());
}

bool GridStatusImpl::AreCellsValid(std::vector<SharedCell> const& cellsSet, Grid const& grid) const
{
    auto isCellValueValid = [&](SharedCell const& cell){ return IsCellValueValid(*cell, grid); };

    return boost::algorithm::all_of(cellsSet, isCellValueValid);
}

