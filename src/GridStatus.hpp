#pragma once

#include <memory>
#include <vector>

#include "Cell.hpp"

namespace sudoku
{

class Grid;
class RelatedCellsGetter;

class GridStatus
{
public:
    virtual ~GridStatus() = default;

    virtual bool AreAllCellsSet(Grid const& grid) const = 0;
    virtual bool AreSetCellsValid(Grid const& grid) const = 0;
};

class GridStatusImpl : public GridStatus
{
public:
    GridStatusImpl(std::unique_ptr<RelatedCellsGetter> relatedCellsGetter);

    bool AreAllCellsSet(Grid const& grid) const override;
    bool AreSetCellsValid(Grid const& grid) const override;

private:
    std::vector<SharedCell> GetRelatedCells(Position const& selectedPosition, Grid const& grid) const;
    std::vector<Value> GetRelatedCellsSetValue(Position const& selectedPosition, Grid const& grid) const;
    bool IsCellValueValid(Cell const& cell, Grid const& grid) const;
    bool AreCellsValid(std::vector<SharedCell> const& cellsSet, Grid const& grid) const;

    std::unique_ptr<RelatedCellsGetter> m_RelatedCellsGetter;
};

} /* namespace sudoku */

