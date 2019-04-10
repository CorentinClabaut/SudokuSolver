#include "GridSolverFactory.hpp"

#include "GridSolverWithoutHypothesis.hpp"
#include "ParallelPossibilitiesRemover.hpp"
#include "ParallelUniquePossibilitySetter.hpp"
#include "PossibilitiesRemover.hpp"
#include "RelatedPositionsGetter.hpp"
#include "UniquePossibilitySetter.hpp"
#include "UniquePossibilityFinder.hpp"
#include "ThreadPool.hpp"

using namespace sudoku;

std::unique_ptr<GridSolver> GridSolverFactory::Make(int parallelThreadsCount)
{
    auto threadPool = std::make_shared<ThreadPool>(parallelThreadsCount);

    return std::make_unique<GridSolverWithHypothesisImpl>(
                std::make_unique<GridSolverWithoutHypothesisImpl>
                (
                    std::make_unique<ParallelPossibilitiesRemoverImpl>(
                        parallelThreadsCount,
                        threadPool,
                        std::make_unique<PossibilitiesRemoverImpl>(
                            std::make_unique<RelatedPositionsGetterImpl>()
                        )
                    ),
                    std::make_unique<ParallelUniquePossibilitySetterImpl>(
                        parallelThreadsCount,
                        threadPool,
                        std::make_unique<UniquePossibilitySetterImpl>(
                            std::make_unique<UniquePossibilityFinderImpl>(
                                std::make_unique<RelatedPositionsGetterImpl>()
                            )
                        )
                    )
                )
            );
}
