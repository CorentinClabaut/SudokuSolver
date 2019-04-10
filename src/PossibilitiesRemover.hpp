#pragma once

#include <memory>

namespace sudoku
{

class Grid;
class Position;
class RelatedPositionsGetter;
struct FoundPositions;

class PossibilitiesRemover
{
public:
    virtual ~PossibilitiesRemover() = default;

    virtual void UpdateGrid(Position const& newFoundPosition, Grid& grid, FoundPositions& foundPositions) const = 0;
};

class PossibilitiesRemoverImpl : public PossibilitiesRemover
{
public:
    PossibilitiesRemoverImpl(std::unique_ptr<RelatedPositionsGetter> relatedPositionsGetter);

    void UpdateGrid(Position const& newFoundPosition, Grid& grid, FoundPositions& foundPositions) const override;

private:
    std::unique_ptr<RelatedPositionsGetter> m_RelatedPositionsGetter;
};

} /* namespace sudoku */

