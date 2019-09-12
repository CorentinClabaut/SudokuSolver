#include "GridSolverFactory.hpp"

#include "GridSolverWithoutHypothesis.hpp"
#include "GridPossibilitiesUpdater.hpp"
#include "UniquePossibilitySetter.hpp"
#include "RelatedPossibilitiesRemover.hpp"

using namespace sudoku;

std::unique_ptr<GridSolver> GridSolverFactory::Make()
{
    return std::make_unique<GridSolverWithHypothesisImpl>(
                std::make_unique<GridSolverWithoutHypothesisImpl>
                (
                    std::make_unique<GridPossibilitiesUpdaterImpl>(
                        std::make_unique<RelatedPossibilitiesRemoverImpl>()
                    ),
                    std::make_unique<UniquePossibilitySetterImpl>()
                )
            );
}
