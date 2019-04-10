#include "UniquePossibilitySetter.hpp"

#include "UniquePossibilityFinder.hpp"
#include "FoundPositions.hpp"
#include "Grid.hpp"

using namespace sudoku;

UniquePossibilitySetterImpl::UniquePossibilitySetterImpl(
        std::unique_ptr<UniquePossibilityFinder> uniquePossibilityFinder) :
    m_UniquePossibilityFinder(std::move(uniquePossibilityFinder))
{}

void UniquePossibilitySetterImpl::SetIfUniquePossibility(Position const& position, Grid& grid, FoundPositions& foundPositions) const
{
    auto& cell = grid.GetCell(position);

    if (cell.IsSet())
        return;

    auto uniquePossibility = m_UniquePossibilityFinder->FindUniquePossibility(position, grid);

    if (!uniquePossibility)
        return;

    cell.SetValue(*uniquePossibility);

    foundPositions.Enqueue(cell.GetPosition());
}
