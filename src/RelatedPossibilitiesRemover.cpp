#include "RelatedPossibilitiesRemover.hpp"

#include <sstream>

#include <boost/algorithm/cxx11/any_of.hpp>
#include <boost/range.hpp>

#include "Grid.hpp"
#include "Cell.hpp"
#include "Position.hpp"

using namespace sudoku;

namespace
{

std::vector<std::reference_wrapper<Cell>> GetCells(Range<Position> const& positions, Grid& grid)
{
    std::vector<std::reference_wrapper<Cell>> cells;
    cells.reserve(positions.size());

    std::transform(positions.begin(), positions.end(), std::back_inserter(cells),
                   [&grid](auto const& p){ return std::ref(grid.GetCell(p)); });

    return cells;
}

auto PartitionFoundAndNotFoundCells(std::vector<std::reference_wrapper<Cell>>& cells)
{
    const auto middle = std::partition(cells.begin(), cells.end(), [](Cell const& c){ return c.IsSet(); });

    return std::pair{boost::make_iterator_range(cells.begin(), middle), boost::make_iterator_range(middle, cells.end())};
}

template<typename TRange>
void ValidateNoFoundCellSetWithValue(TRange const& foundRelatedCells, Value foundValue)
{
    if (boost::algorithm::any_of(foundRelatedCells, [foundValue](Cell const& c){ return c.GetValue() == foundValue; }))
        throw std::runtime_error("related cell already has new found cell value.");
}

template<typename TRange>
void UpdateRelatedCellsPossibilities(TRange const& notFoundRelatedCells, Value foundValue, FoundPositions& foundPositions)
{
    for (Cell& cell : notFoundRelatedCells)
    {
        cell.RemovePossibility(foundValue);

        if (cell.IsSet())
        {
            foundPositions.push(cell.GetPosition());
        }
    }
}

} // anonymous namespace

void RelatedPossibilitiesRemoverImpl::UpdateRelatedPossibilities(Position const& newFoundPosition, Grid& grid, FoundPositions& foundPositions) const
{
    const auto foundValue = grid.GetCell(newFoundPosition).GetValue();

    if (!foundValue)
    {
        std::stringstream error;
        error << "Can't update grid possibilities because: cell '" << newFoundPosition << "' has no value set";
        throw std::runtime_error(error.str());
    }

    const auto relatedPositions = m_RelatedPositionsGetter.GetAllRelatedPositions(newFoundPosition, grid.GetGridSize());
    auto relatedCells = GetCells(relatedPositions, grid);

    auto const& [relatedFoundCells, relatedNotFoundCells] = PartitionFoundAndNotFoundCells(relatedCells);

    ValidateNoFoundCellSetWithValue(relatedFoundCells, *foundValue);

    UpdateRelatedCellsPossibilities(relatedNotFoundCells, *foundValue, foundPositions);
}
