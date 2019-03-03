#pragma once

#include <vector>

#include "Cell.hpp"

namespace sudoku
{

class UniquePossibilityFinder;
struct FoundCells;
struct Grid;

class UniquePossibilitySetter
{
public:
    virtual ~UniquePossibilitySetter() = default;

    virtual void SetCellsWithUniquePossibility(std::vector<SharedCell>& cells, Grid const& grid, FoundCells& foundCells) const = 0;
};

class UniquePossibilitySetterImpl : public UniquePossibilitySetter
{
public:
    UniquePossibilitySetterImpl(
            std::unique_ptr<UniquePossibilityFinder> uniquePossibilityFinder);

    void SetCellsWithUniquePossibility(std::vector<SharedCell>& cells, Grid const& grid, FoundCells& foundCells) const override;

private:
    std::unique_ptr<UniquePossibilityFinder> m_UniquePossibilityFinder;
};

} /* namespace sudoku */

