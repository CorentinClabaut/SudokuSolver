#pragma once

#include <memory>
#include <vector>

#include "Cell.hpp"
#include "RelatedPositionsGetter.hpp"

namespace sudoku
{

class Grid;
enum class GridStatus;

class GridStatusGetter
{
public:
    virtual ~GridStatusGetter() = default;

    virtual GridStatus GetStatus(Grid& grid) const = 0;
};

class GridStatusGetterImpl : public GridStatusGetter
{
public:
    GridStatus GetStatus(Grid& grid) const override;

private:
    bool AreSetCellsValid(Grid& grid) const;
    std::vector<std::reference_wrapper<Cell>> GetRelatedCells(Position const& selectedPosition, Grid& grid) const;
    std::vector<Value> GetRelatedCellsSetValue(Position const& selectedPosition, Grid& grid) const;
    bool IsCellValueValid(Cell const& cell, Grid& grid) const;
    bool AreCellsValid(std::vector<std::reference_wrapper<Cell>> const& cellsSet, Grid& grid) const;

    const RelatedPositionsGetterImpl m_RelatedPositionsGetter;
};

} /* namespace sudoku */

