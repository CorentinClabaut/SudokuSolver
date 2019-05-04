#include "GridSolverWithHypothesis.hpp"

#include "GridSolverWithoutHypothesis.hpp"
#include "GridStatus.hpp"
#include "Grid.hpp"

#include <limits>

using namespace sudoku;

namespace
{

void GetFoundPositions(Grid const& grid, FoundPositions& foundPositions)
{
    for (auto cell : grid)
    {
        if (cell.IsSet())
            foundPositions.push(cell.GetPosition());
    }
}

Position SelectBestPositionForHypothesis(Grid const& grid)
{
    int minimumNumberPossibilities = std::numeric_limits<int>::max();
    Position bestPosition;

    for (auto const& cell : grid)
    {
        const auto possibilitiesCount = cell.GetNumberPossibilitiesLeft();

        if (possibilitiesCount == 1)
            continue;

        if (possibilitiesCount < minimumNumberPossibilities)
        {
            minimumNumberPossibilities = possibilitiesCount;
            bestPosition = cell.GetPosition();
        }
    }

    if (minimumNumberPossibilities == std::numeric_limits<int>::max())
        throw std::runtime_error("Can't find best position for hyposesis in completed grid.");

    return bestPosition;
}

Value SelectHypothesisValue(Grid& grid, Position const& hypothesisCellPosition)
{
    auto const& possibilities = grid.GetCell(hypothesisCellPosition).GetPossibilities();

    return possibilities.GetPossibilityLeft();
}

void SetHypotheticCellValue(Grid& grid, FoundPositions& foundPositions, Position const& hypothesisCellPosition, Value valueToTry)
{
    auto& hypothesisCell = grid.GetCell(hypothesisCellPosition);

    hypothesisCell.SetValue(valueToTry);
    foundPositions.push(hypothesisCell.GetPosition());
}

bool CellHasOnlyOnePossibilityLeft(Grid& grid, Position const& position)
{
    return grid.GetCell(position).GetNumberPossibilitiesLeft() == 1;
}

void RemoveWrongHypotheticCellValue(Grid& gridBeforeHypothesis, Position const& hypothesisCellPosition, Value triedValue)
{
    gridBeforeHypothesis.GetCell(hypothesisCellPosition).RemovePossibility(triedValue);
}

} // anonymous namespace

GridSolverWithHypothesisImpl::GridSolverWithHypothesisImpl(
        std::unique_ptr<GridSolverWithoutHypothesis> gridSolverWithoutHypothesis) :
    m_GridSolverWithoutHypothesis(std::move(gridSolverWithoutHypothesis))
{}

bool GridSolverWithHypothesisImpl::Solve(Grid& grid) const
{
    FoundPositions foundPositions;
    GetFoundPositions(grid, foundPositions);

    return SolveWithtHypothesis(grid, foundPositions);
}

bool GridSolverWithHypothesisImpl::SolveWithtHypothesis(Grid& grid, FoundPositions& foundPositions) const
{
    auto status = m_GridSolverWithoutHypothesis->Solve(grid, foundPositions);

    if (status == GridStatus::SolvedCorrectly)
        return true;

    if (status == GridStatus::Wrong)
        return false;

    auto gridBeforeHypothesis {grid};

    const auto hypothesisCellPosition = SelectBestPositionForHypothesis(grid);

    while (true)
    {
        const auto triedValue = SelectHypothesisValue(grid, hypothesisCellPosition);

        SetHypotheticCellValue(grid, foundPositions, hypothesisCellPosition, triedValue);

        bool solvedCorrectly = SolveWithtHypothesis(grid, foundPositions);

        if (solvedCorrectly)
            return true;

        if (CellHasOnlyOnePossibilityLeft(gridBeforeHypothesis, hypothesisCellPosition))
            return false;

        RemoveWrongHypotheticCellValue(gridBeforeHypothesis, hypothesisCellPosition, triedValue);
        grid = gridBeforeHypothesis;
    }
}
