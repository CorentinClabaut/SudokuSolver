#pragma once

#include <vector>

#include "Cell.hpp"

namespace sudoku
{

class UniquePossibilityFinder;
class FoundCellsEnqueuer;
struct Grid;

class UniquePossibilitySetter
{
public:
    virtual ~UniquePossibilitySetter() = default;

    virtual void SetCellsWithUniquePossibility(std::vector<SharedCell>& cells, Grid const& grid) = 0;
};

class UniquePossibilitySetterImpl : public UniquePossibilitySetter
{
public:
    UniquePossibilitySetterImpl(
            std::unique_ptr<UniquePossibilityFinder> uniquePossibilityFinder,
            std::shared_ptr<FoundCellsEnqueuer> foundCellsEnqueuer);

    void SetCellsWithUniquePossibility(std::vector<SharedCell>& cells, Grid const& grid) override;

private:
    std::unique_ptr<UniquePossibilityFinder> m_UniquePossibilityFinder;
    std::shared_ptr<FoundCellsEnqueuer> m_FoundCellsEnqueuer;
};

} /* namespace sudoku */

