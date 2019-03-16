#include "GridSolverWithoutHypothesis.hpp"

#include "ParallelPossibilitiesRemover.hpp"
#include "ParallelUniquePossibilitySetter.hpp"
#include "FoundCells.hpp"
#include "GridStatus.hpp"
#include "Grid.hpp"

using namespace sudoku;

namespace
{
bool AreAllCellsSet(Grid const& grid)
{
    auto isSet = [](SharedCell const& cell)
    {
        return cell->GetValue().has_value();
    };

    return std::all_of(grid.begin(), grid.end(), isSet);
}
} // anonymous namespace

GridSolverWithoutHypothesisImpl::GridSolverWithoutHypothesisImpl(
        std::unique_ptr<ParallelPossibilitiesRemover> parallelPossibilitiesRemover,
        std::unique_ptr<ParallelUniquePossibilitySetter> parallelUniquePossibilitySetter) :
    m_ParallelPossibilitiesRemover(std::move(parallelPossibilitiesRemover)),
    m_ParallelUniquePossibilitySetter(std::move(parallelUniquePossibilitySetter))
{}

GridStatus GridSolverWithoutHypothesisImpl::Solve(Grid& grid, FoundCells& foundCells) const
{
    if (foundCells.m_Queue.empty())
        throw std::runtime_error("Can't solve without hypothesis if no cell has been found");

    try
    {
        while(!foundCells.m_Queue.empty())
        {
            m_ParallelPossibilitiesRemover->UpdateGrid(foundCells, grid);

            m_ParallelUniquePossibilitySetter->SetCellsWithUniquePossibility(grid, foundCells);
        }
    }
    catch(std::exception const&)
    {
        while(!foundCells.m_Queue.empty())
            foundCells.m_Queue.pop();

        return GridStatus::Wrong;
    }

    return AreAllCellsSet(grid) ? GridStatus::SolvedCorrectly : GridStatus::Incomplete;
}
