#include "PossibilitiesRemover.hpp"

#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext.hpp>

#include "Grid.hpp"
#include "Cell.hpp"
#include "RelatedCellsGetter.hpp"
#include "FoundCellsEnqueuer.hpp"

using namespace sudoku;

namespace
{
bool CellFound(SharedCell cell)
{
    return cell->GetValue().has_value();
}
bool CellNotFound(SharedCell cell)
{
    return !cell->GetValue().has_value();
}
} // anonymous namespace

PossibilitiesRemoverImpl::PossibilitiesRemoverImpl(std::unique_ptr<RelatedCellsGetter> relatedCellsGetter) :
    m_RelatedCellsGetter(std::move(relatedCellsGetter))
{}

void PossibilitiesRemoverImpl::UpdateGrid(Cell const& newFoundCell, Grid& grid, FoundCellsEnqueuer& foundCellsEnqueuer) const
{
    auto foundValue = newFoundCell.GetValue();

    if (!foundValue)
    {
        std::stringstream error;
        error << "Can't update grid possibilities because: cell '" << newFoundCell.GetPosition() << "' has no value set";
        throw std::runtime_error(error.str());
    }

    auto relatedCells = m_RelatedCellsGetter->GetAllRelatedCells(newFoundCell.GetPosition(), grid);
    boost::range::remove_erase_if(relatedCells, CellFound);

    boost::for_each(relatedCells, [=](auto& cell){ cell->RemovePossibility(*foundValue); });

    boost::range::remove_erase_if(relatedCells, CellNotFound);

    boost::for_each(relatedCells, [&](auto cell){ foundCellsEnqueuer.Enqueue(cell); });
}
