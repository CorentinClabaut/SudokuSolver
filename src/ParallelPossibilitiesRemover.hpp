#pragma once

#include <memory>

namespace sudoku
{

class PossibilitiesRemover;
class FoundCells;
class Grid;

class ParallelPossibilitiesRemover
{
public:
    virtual ~ParallelPossibilitiesRemover() = default;

    virtual void UpdateGrid(FoundCells& foundCells, Grid& grid) const = 0;
};

class ParallelPossibilitiesRemoverImpl : public ParallelPossibilitiesRemover
{
public:
    ParallelPossibilitiesRemoverImpl(
            int parallelThreadsCount,
            std::unique_ptr<PossibilitiesRemover> possibilitiesRemover);

    void UpdateGrid(FoundCells& foundCells, Grid& grid) const override;

private:
    void RemoveQueuedUnvalidPossibilities(FoundCells& foundCells, Grid& grid, int& threadsWorkingCount) const;

    const int m_ParallelThreadsCount;

    std::unique_ptr<PossibilitiesRemover> m_PossibilitiesRemover;
};

} /* namespace sudoku */
