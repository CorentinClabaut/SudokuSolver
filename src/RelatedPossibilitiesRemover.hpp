#pragma once

#include <memory>

#include "FoundPositions.hpp"
#include "RelatedPositionsGetter.hpp"

namespace sudoku
{

class Grid;
class Position;

class RelatedPossibilitiesRemover
{
public:
    virtual ~RelatedPossibilitiesRemover() = default;

    virtual void UpdateRelatedPossibilities(Position const& newFoundPosition, Grid& grid, FoundPositions& foundPositions) const = 0;
};

class RelatedPossibilitiesRemoverImpl : public RelatedPossibilitiesRemover
{
public:
    void UpdateRelatedPossibilities(Position const& newFoundPosition, Grid& grid, FoundPositions& foundPositions) const override;

private:
    const RelatedPositionsGetterImpl m_RelatedPositionsGetter;
};

} /* namespace sudoku */

