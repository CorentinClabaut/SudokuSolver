#include "GridSolverWithoutHypothesis.hpp"

#include "ParallelPossibilitiesRemover.hpp"
#include "ParallelUniquePossibilitySetter.hpp"
#include "FoundPositions.hpp"
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
        std::unique_ptr<ParallelPossibilitiesRemover> parallelPossibilitiesRemover,
        std::unique_ptr<ParallelUniquePossibilitySetter> parallelUniquePossibilitySetter) :
    m_ParallelPossibilitiesRemover(std::move(parallelPossibilitiesRemover)),
    m_ParallelUniquePossibilitySetter(std::move(parallelUniquePossibilitySetter))
{}

GridStatus GridSolverWithoutHypothesisImpl::Solve(Grid& grid, FoundPositions& foundPositions) const
{
    if (foundPositions.m_Queue.empty())
        throw std::runtime_error("Can't solve without hypothesis if no cell has been found");

    try
    {
        while(!foundPositions.m_Queue.empty())
        {
            m_ParallelPossibilitiesRemover->UpdateGrid(foundPositions, grid);

            m_ParallelUniquePossibilitySetter->SetCellsWithUniquePossibility(grid, foundPositions);
        }
    }
    catch(std::exception const&)
    {
        while(!foundPositions.m_Queue.empty())
            foundPositions.m_Queue.pop();

        return GridStatus::Wrong;
    }

    return AreAllCellsSet(grid) ? GridStatus::SolvedCorrectly : GridStatus::Incomplete;
}
