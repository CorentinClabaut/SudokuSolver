#include "UniquePossibilitySetter.hpp"

#include "UniquePossibilityFinder.hpp"
#include "FoundCells.hpp"
#include "Grid.hpp"

using namespace sudoku;

UniquePossibilitySetterImpl::UniquePossibilitySetterImpl(
        std::unique_ptr<UniquePossibilityFinder> uniquePossibilityFinder) :
    m_UniquePossibilityFinder(std::move(uniquePossibilityFinder))
{}

void UniquePossibilitySetterImpl::SetCellsWithUniquePossibility(std::vector<SharedCell>& cells, Grid const& grid, FoundCells& foundCells) const
{
    for (auto cell : cells)
    {
        if (cell->IsSet())
            continue;

        auto uniquePossibility = m_UniquePossibilityFinder->FindUniquePossibility(cell->GetPosition(), grid);

        if (!uniquePossibility)
            continue;

        cell->SetValue(*uniquePossibility);

        foundCells.Enqueue(cell);
    }
}
