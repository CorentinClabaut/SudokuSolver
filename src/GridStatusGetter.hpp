#pragma once

#include <memory>
#include <vector>

#include "Cell.hpp"

namespace sudoku
{

class Grid;
class RelatedCellsGetter;
enum class GridStatus;

class GridStatusGetter
{
public:
    virtual ~GridStatusGetter() = default;

    virtual GridStatus GetStatus(Grid const& grid) const = 0;
};

class GridStatusGetterImpl : public GridStatusGetter
{
public:
    GridStatusGetterImpl(std::unique_ptr<RelatedCellsGetter> relatedCellsGetter);

    GridStatus GetStatus(Grid const& grid) const override;

private:
    bool AreSetCellsValid(Grid const& grid) const;
    std::vector<SharedCell> GetRelatedCells(Position const& selectedPosition, Grid const& grid) const;
    std::vector<Value> GetRelatedCellsSetValue(Position const& selectedPosition, Grid const& grid) const;
    bool IsCellValueValid(Cell const& cell, Grid const& grid) const;
    bool AreCellsValid(std::vector<SharedCell> const& cellsSet, Grid const& grid) const;

    std::unique_ptr<RelatedCellsGetter> m_RelatedCellsGetter;
};

} /* namespace sudoku */

