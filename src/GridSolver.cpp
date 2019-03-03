#include "GridSolver.hpp"

#include "ParallelPossibilitiesRemover.hpp"
#include "ParallelUniquePossibilitySetter.hpp"
#include "GridStatusGetter.hpp"
#include "FoundCells.hpp"
#include "GridStatus.hpp"
#include "Grid.hpp"

using namespace sudoku;

GridSolverImpl::GridSolverImpl(
        std::unique_ptr<ParallelPossibilitiesRemover> parallelPossibilitiesRemover,
        std::unique_ptr<ParallelUniquePossibilitySetter> parallelUniquePossibilitySetter,
        std::unique_ptr<GridStatusGetter> gridStatusGetter) :
    m_ParallelPossibilitiesRemover(std::move(parallelPossibilitiesRemover)),
    m_ParallelUniquePossibilitySetter(std::move(parallelUniquePossibilitySetter)),
    m_GridStatusGetter(std::move(gridStatusGetter))
{}

GridStatus GridSolverImpl::Solve(Grid& grid) const
{
    FoundCells foundCells;
    GetFoundCells(grid, foundCells);

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

void GridSolverImpl::GetFoundCells(Grid const& grid, FoundCells& foundCells) const
{
    for (auto cell : grid)
    {
        auto value = cell->GetValue();

        if (value)
            foundCells.m_Queue.push(cell);
    }
}
