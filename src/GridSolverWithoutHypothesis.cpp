#include "GridSolverWithoutHypothesis.hpp"

#include "GridPossibilitiesUpdater.hpp"
#include "UniquePossibilitySetter.hpp"
#include "GridStatus.hpp"
#include "Grid.hpp"

using namespace sudoku;

namespace
{
bool AreAllCellsSet(Grid const& grid)
{
    return std::all_of(grid.begin(), grid.end(), [](auto const& cell){ return cell.IsSet(); });
}
} // anonymous namespace

GridSolverWithoutHypothesisImpl::GridSolverWithoutHypothesisImpl(
        std::unique_ptr<GridPossibilitiesUpdater> gridPossibilitiesUpdater,
        std::unique_ptr<UniquePossibilitySetter> uniquePossibilitySetter) :
    m_GridPossibilitiesUpdater(std::move(gridPossibilitiesUpdater)),
    m_UniquePossibilitySetter(std::move(uniquePossibilitySetter))
{}

GridStatus GridSolverWithoutHypothesisImpl::Solve(Grid& grid, FoundPositions& foundPositions) const
{
    if (foundPositions.empty())
        throw std::runtime_error("Can't solve without hypothesis if no cell has been found");

    try
    {
        while(!foundPositions.empty())
        {
            m_GridPossibilitiesUpdater->UpdateGrid(foundPositions, grid);

            m_UniquePossibilitySetter->SetCellsWithUniquePossibility(grid, foundPositions);
        }
    }
    catch(std::exception const&)
    {
        while(!foundPositions.empty())
            foundPositions.pop();

        return GridStatus::Wrong;
    }

    return AreAllCellsSet(grid) ? GridStatus::SolvedCorrectly : GridStatus::Incomplete;
}
