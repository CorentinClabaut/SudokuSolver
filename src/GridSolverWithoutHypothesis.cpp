#include "GridSolverWithoutHypothesis.hpp"

#include "ParallelPossibilitiesRemover.hpp"
#include "ParallelUniquePossibilitySetter.hpp"
#include "GridStatusGetter.hpp"
#include "FoundCells.hpp"
#include "GridStatus.hpp"
#include "Grid.hpp"

using namespace sudoku;

GridSolverWithoutHypothesisImpl::GridSolverWithoutHypothesisImpl(
        std::unique_ptr<ParallelPossibilitiesRemover> parallelPossibilitiesRemover,
        std::unique_ptr<ParallelUniquePossibilitySetter> parallelUniquePossibilitySetter,
        std::unique_ptr<GridStatusGetter> gridStatusGetter) :
    m_ParallelPossibilitiesRemover(std::move(parallelPossibilitiesRemover)),
    m_ParallelUniquePossibilitySetter(std::move(parallelUniquePossibilitySetter)),
    m_GridStatusGetter(std::move(gridStatusGetter))
{}

GridStatus GridSolverWithoutHypothesisImpl::Solve(Grid& grid, FoundCells& foundCells) const
{
    while(!foundCells.m_Queue.empty())
    {
        if (auto gridStatus = m_GridStatusGetter->GetStatus(grid);
            gridStatus == GridStatus::SolvedCorrectly || gridStatus == GridStatus::Wrong)
        {
            return gridStatus;
        }

        m_ParallelPossibilitiesRemover->UpdateGrid(foundCells, grid);

        if (auto gridStatus = m_GridStatusGetter->GetStatus(grid);
            gridStatus == GridStatus::SolvedCorrectly || gridStatus == GridStatus::Wrong)
        {
            return gridStatus;
        }

        m_ParallelUniquePossibilitySetter->SetCellsWithUniquePossibility(grid, foundCells);
    }

    return GridStatus::Incomplete;
}
