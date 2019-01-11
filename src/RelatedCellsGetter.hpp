#pragma once

#include "CellsGroup.hpp"

namespace sudoku
{

class Position;
class Grid;

class RelatedCellsGetter
{
public:
    virtual ~RelatedCellsGetter() = default;

    virtual CellsGroup GetRelatedHorizontalCells(Position const& selectedPosition, Grid const& grid) const = 0;
    virtual CellsGroup GetRelatedVerticalCells(Position const& selectedPosition, Grid const& grid) const = 0;
    virtual CellsGroup GetRelatedBlockCells(Position const& selectedPosition, Grid const& grid) const = 0;
};

class RelatedCellsGetterImpl : public RelatedCellsGetter
{
public:
    CellsGroup GetRelatedHorizontalCells(Position const& selectedPosition, Grid const& grid) const override;
    CellsGroup GetRelatedVerticalCells(Position const& selectedPosition, Grid const& grid) const override;
    CellsGroup GetRelatedBlockCells(Position const& selectedPosition, Grid const& grid) const override;
};

} /* namespace sudoku */

