#pragma once

#include <memory>

namespace sudoku
{

class Grid;
class Cell;
class RelatedCellsGetter;
struct FoundCells;

class PossibilitiesRemover
{
public:
    virtual ~PossibilitiesRemover() = default;

    virtual void UpdateGrid(Cell const& newFoundCell, Grid& grid, FoundCells& foundCells) const = 0;
};

class PossibilitiesRemoverImpl : public PossibilitiesRemover
{
public:
    PossibilitiesRemoverImpl(std::unique_ptr<RelatedCellsGetter> relatedCellsGetter);

    void UpdateGrid(Cell const& newFoundCell, Grid& grid, FoundCells& foundCells) const override;

private:
    std::unique_ptr<RelatedCellsGetter> m_RelatedCellsGetter;
};

} /* namespace sudoku */

