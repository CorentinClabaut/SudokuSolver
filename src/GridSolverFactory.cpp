#include "GridSolverFactory.hpp"

#include "GridSolverWithoutHypothesis.hpp"
#include "ParallelPossibilitiesRemover.hpp"
#include "ParallelUniquePossibilitySetter.hpp"
#include "GridStatusGetter.hpp"
#include "PossibilitiesRemover.hpp"
#include "RelatedCellsGetter.hpp"
#include "UniquePossibilitySetter.hpp"
#include "UniquePossibilityFinder.hpp"

using namespace sudoku;

std::unique_ptr<GridSolver> GridSolverFactory::Make(int parallelThreadsCount)
{
    return std::make_unique<GridSolverWithHypothesisImpl>(
                std::make_unique<GridSolverWithoutHypothesisImpl>
                (
                    std::make_unique<ParallelPossibilitiesRemoverImpl>(
                        parallelThreadsCount,
                        std::make_unique<PossibilitiesRemoverImpl>(
                            std::make_unique<RelatedCellsGetterImpl>()
                        )
                    ),
                    std::make_unique<ParallelUniquePossibilitySetterImpl>(
                        parallelThreadsCount,
                        std::make_unique<UniquePossibilitySetterImpl>(
                            std::make_unique<UniquePossibilityFinderImpl>(
                                std::make_unique<RelatedCellsGetterImpl>()
                            )
                        )
                    ),
                    std::make_unique<GridStatusGetterImpl>(
                        std::make_unique<RelatedCellsGetterImpl>()
                    )
                )
            );
}
