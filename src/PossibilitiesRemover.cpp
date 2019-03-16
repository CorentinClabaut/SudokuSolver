#include "PossibilitiesRemover.hpp"

#include <boost/algorithm/cxx11/any_of.hpp>
#include <boost/range.hpp>

#include "Grid.hpp"
#include "Cell.hpp"
#include "RelatedCellsGetter.hpp"
#include "FoundCells.hpp"

using namespace sudoku;

namespace
{

bool HasValue(SharedCell cell)
{
    return cell->GetValue().has_value();
}

auto PartitionFoundAndNotFoundCells(std::vector<SharedCell>& cells)
{
    const auto middle = std::partition(cells.begin(), cells.end(), HasValue);

    return std::pair{boost::make_iterator_range(cells.begin(), middle), boost::make_iterator_range(middle, cells.end())};
}

template<typename TRange>
void ValidateNoFoundCellSetWithValue(TRange const& foundRelatedCells, Value foundValue)
{
    if (boost::algorithm::any_of(foundRelatedCells, [foundValue](const auto cell){ return cell->GetValue() == foundValue; }))
        throw std::runtime_error("related cell already has new found cell value.");
}

template<typename TRange>
void UpdateRelatedCellsPossibilities(TRange const& notFoundRelatedCells, Value foundValue, FoundCells& foundCells)
{
    for (auto cell : notFoundRelatedCells)
    {
        cell->RemovePossibility(foundValue);

        if (HasValue(cell))
        {
            foundCells.Enqueue(cell);
        }
    }
}

} // anonymous namespace

PossibilitiesRemoverImpl::PossibilitiesRemoverImpl(std::unique_ptr<RelatedCellsGetter> relatedCellsGetter) :
    m_RelatedCellsGetter(std::move(relatedCellsGetter))
{}

void PossibilitiesRemoverImpl::UpdateGrid(Cell const& newFoundCell, Grid& grid, FoundCells& foundCells) const
{
    const auto foundValue = newFoundCell.GetValue();

    if (!foundValue)
    {
        std::stringstream error;
        error << "Can't update grid possibilities because: cell '" << newFoundCell.GetPosition() << "' has no value set";
        throw std::runtime_error(error.str());
    }

    auto relatedCells = m_RelatedCellsGetter->GetAllRelatedCells(newFoundCell.GetPosition(), grid);

    const auto& [relatedFoundCells, relatedNotFoundCells] = PartitionFoundAndNotFoundCells(relatedCells);

    ValidateNoFoundCellSetWithValue(relatedFoundCells, *foundValue);

    UpdateRelatedCellsPossibilities(relatedNotFoundCells, *foundValue, foundCells);
}
