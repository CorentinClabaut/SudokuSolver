#pragma once

#include <memory>

#include "FoundPositions.hpp"

namespace sudoku
{

class RelatedPossibilitiesRemover;
class Grid;

class GridPossibilitiesUpdater
{
public:
    virtual ~GridPossibilitiesUpdater() = default;

    virtual void UpdateGrid(FoundPositions& foundPositions, Grid& grid) = 0;
};

class GridPossibilitiesUpdaterImpl : public GridPossibilitiesUpdater
{
public:
    GridPossibilitiesUpdaterImpl(
            std::unique_ptr<RelatedPossibilitiesRemover> relatedPossibilitiesRemover);

    void UpdateGrid(FoundPositions& foundPositions, Grid& grid) override;

private:
    std::unique_ptr<RelatedPossibilitiesRemover> m_RelatedPossibilitiesRemover;
};

} /* namespace sudoku */

