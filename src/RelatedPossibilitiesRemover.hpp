#pragma once

#include <memory>

#include "FoundPositions.hpp"

namespace sudoku
{

class Grid;
class Position;
class RelatedPositionsGetter;

class RelatedPossibilitiesRemover
{
public:
    virtual ~RelatedPossibilitiesRemover() = default;

    virtual void UpdateRelatedPossibilities(Position const& newFoundPosition, Grid& grid, FoundPositions& foundPositions) const = 0;
};

class RelatedPossibilitiesRemoverImpl : public RelatedPossibilitiesRemover
{
public:
    RelatedPossibilitiesRemoverImpl(std::unique_ptr<RelatedPositionsGetter> relatedPositionsGetter);

    void UpdateRelatedPossibilities(Position const& newFoundPosition, Grid& grid, FoundPositions& foundPositions) const override;

private:
    std::unique_ptr<RelatedPositionsGetter> m_RelatedPositionsGetter;
};

} /* namespace sudoku */

