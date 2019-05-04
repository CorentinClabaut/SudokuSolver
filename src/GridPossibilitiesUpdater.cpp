#include "GridPossibilitiesUpdater.hpp"

#include "RelatedPossibilitiesRemover.hpp"
#include "Grid.hpp"

using namespace sudoku;

GridPossibilitiesUpdaterImpl::GridPossibilitiesUpdaterImpl(
        std::unique_ptr<RelatedPossibilitiesRemover> relatedPossibilitiesRemover) :
    m_RelatedPossibilitiesRemover(std::move(relatedPossibilitiesRemover))
{}

void GridPossibilitiesUpdaterImpl::UpdateGrid(FoundPositions& foundPositions, Grid& grid)
{
    while(!foundPositions.empty())
    {
        auto newFoundPosition = foundPositions.front();
        foundPositions.pop();
        m_RelatedPossibilitiesRemover->UpdateRelatedPossibilities(newFoundPosition, grid, foundPositions);
    }
}
