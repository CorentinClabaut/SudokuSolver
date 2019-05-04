#include "UniquePossibilitySetter.hpp"

#include "Grid.hpp"
#include "UniquePossibilityFinder.hpp"

using namespace sudoku;

UniquePossibilitySetterImpl::UniquePossibilitySetterImpl(
        std::unique_ptr<UniquePossibilityFinder> uniquePossibilityFinder) :
    m_UniquePossibilityFinder(std::move(uniquePossibilityFinder))
{}

void UniquePossibilitySetterImpl::SetCellsWithUniquePossibility(Grid& grid, FoundPositions& foundPositions)
{
    for (auto& cell : grid)
    {
        if (cell.IsSet())
            continue;

        auto uniquePossibility = m_UniquePossibilityFinder->FindUniquePossibility(cell, grid);

        if (!uniquePossibility)
            continue;

        cell.SetValue(*uniquePossibility);
        foundPositions.push(cell.GetPosition());
    }
}
