#include "UniquePossibilitySetter.hpp"

#include "UniquePossibilityFinder.hpp"
#include "FoundCellsEnqueuer.hpp"
#include "Grid.hpp"

using namespace sudoku;

UniquePossibilitySetterImpl::UniquePossibilitySetterImpl(
        std::unique_ptr<UniquePossibilityFinder> uniquePossibilityFinder,
        std::shared_ptr<FoundCellsEnqueuer> foundCellsEnqueuer) :
    m_UniquePossibilityFinder(std::move(uniquePossibilityFinder)),
    m_FoundCellsEnqueuer(foundCellsEnqueuer)
{}

void UniquePossibilitySetterImpl::SetCellsWithUniquePossibility(std::vector<SharedCell>& cells, Grid const& grid)
{
    for (auto& cell : cells)
    {
        auto uniquePossibility = m_UniquePossibilityFinder->FindUniquePossibility(cell->GetPosition(), grid);

        if (!uniquePossibility)
            continue;

        cell->SetValue(*uniquePossibility);

        m_FoundCellsEnqueuer->Enqueue(cell);
    }
}
