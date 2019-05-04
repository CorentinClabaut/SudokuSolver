#pragma once

#include <memory>

#include "Value.hpp"
#include "FoundPositions.hpp"

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

    virtual void SetCellsWithUniquePossibility(Grid& grid, FoundPositions& foundPositions) = 0;
};

class UniquePossibilitySetterImpl : public UniquePossibilitySetter
{
public:
    UniquePossibilitySetterImpl(
            std::unique_ptr<UniquePossibilityFinder> uniquePossibilityFinder);

    void SetCellsWithUniquePossibility(Grid& grid, FoundPositions& foundPositions) override;

private:
    std::unique_ptr<UniquePossibilityFinder> m_UniquePossibilityFinder;
};

} /* namespace sudoku */

