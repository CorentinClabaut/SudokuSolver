#include "GridSolverWithHypothesis.hpp"

#include "GridSolverWithoutHypothesis.hpp"
#include "FoundCells.hpp"
#include "GridStatus.hpp"
#include "Grid.hpp"

#include <limits>

using namespace sudoku;

namespace
{

void GetFoundCells(Grid const& grid, FoundCells& foundCells)
{
    for (auto cell : grid)
    {
        auto value = cell->GetValue();

        if (value)
            foundCells.m_Queue.push(cell);
    }
}

Position SelectBestPositionForHypothesis(Grid const& grid)
{
    size_t minimumNumberPossibilities = std::numeric_limits<size_t>::max();
    Position bestPosition;

    for (auto const& cell : grid)
    {
        const auto possibilitiesCount = cell->GetPossibilities().size();

        if (possibilitiesCount == 1)
            continue;

        if (possibilitiesCount < minimumNumberPossibilities)
        {
            minimumNumberPossibilities = possibilitiesCount;
            bestPosition = cell->GetPosition();
        }
    }

    return bestPosition;
}

Value SelectHypothesisValue(Grid& grid, Position const& hypothesisCellPosition)
{
    return *grid.GetCell(hypothesisCellPosition)->GetPossibilities().begin();
}

void SetHypotheticCellValue(Grid& grid, FoundCells& foundCells, Position const& hypothesisCellPosition, Value valueToTry)
{
    auto hypothesisCell = grid.GetCell(hypothesisCellPosition);

    hypothesisCell->SetValue(valueToTry);
    foundCells.Enqueue(hypothesisCell);
}

bool CellHasOnlyOnePossibilityLeft(Grid& grid, Position const& position)
{
    return grid.GetCell(position)->GetPossibilities().size() == 1;
}

void RemoveWrongHypotheticCellValue(Grid& gridBeforeHypothesis, Position const& hypothesisCellPosition, Value triedValue)
{
    gridBeforeHypothesis.GetCell(hypothesisCellPosition)->RemovePossibility(triedValue);
}

} // anonymous namespace

GridSolverWithHypothesisImpl::GridSolverWithHypothesisImpl(
        std::unique_ptr<GridSolverWithoutHypothesis> gridSolverWithoutHypothesis) :
    m_GridSolverWithoutHypothesis(std::move(gridSolverWithoutHypothesis))
{}

bool GridSolverWithHypothesisImpl::Solve(Grid& grid) const
{
    FoundCells foundCells;
    GetFoundCells(grid, foundCells);

    return SolveWithtHypothesis(grid, foundCells);
}

bool GridSolverWithHypothesisImpl::SolveWithtHypothesis(Grid& grid, FoundCells& foundCells) const
{
    auto status = m_GridSolverWithoutHypothesis->Solve(grid, foundCells);

    if (status == GridStatus::SolvedCorrectly)
        return true;

    if (status == GridStatus::Wrong)
        return false;

    auto gridBeforeHypothesis {grid};

    const auto hypothesisCellPosition = SelectBestPositionForHypothesis(grid);

    while (true)
    {
        const auto triedValue = SelectHypothesisValue(grid, hypothesisCellPosition);

        SetHypotheticCellValue(grid, foundCells, hypothesisCellPosition, triedValue);

        bool solvedCorrectly = SolveWithtHypothesis(grid, foundCells);

        if (solvedCorrectly)
            return true;

        if (CellHasOnlyOnePossibilityLeft(gridBeforeHypothesis, hypothesisCellPosition))
            return false;

        RemoveWrongHypotheticCellValue(gridBeforeHypothesis, hypothesisCellPosition, triedValue);
        grid = gridBeforeHypothesis;
    }
}
