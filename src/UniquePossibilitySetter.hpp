#pragma once

#include <memory>

#include "Value.hpp"
#include "FoundPositions.hpp"
#include "RelatedPositionsGetter.hpp"

namespace sudoku
{

class Grid;
class UniquePossibilityFinder;
class PositionIter;
struct Position;

class UniquePossibilitySetter
{
public:
    virtual ~UniquePossibilitySetter() = default;

    virtual void SetCellsWithUniquePossibility(Grid& grid, FoundPositions& foundPositions) const = 0;
};

class UniquePossibilitySetterImpl : public UniquePossibilitySetter
{
public:
    void SetCellsWithUniquePossibility(Grid& grid, FoundPositions& foundPositions) const override;

private:
    const RelatedPositionsGetterImpl m_RelatedPositionsGetter;
};

} /* namespace sudoku */

