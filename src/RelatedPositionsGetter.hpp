#pragma once

#include "Position.hpp"

namespace sudoku
{

class Position;

class RelatedPositionsGetter
{
public:
    virtual ~RelatedPositionsGetter() = default;

    virtual std::vector<Position> GetRelatedHorizontalPositions(Position const& selectedPosition, int gridSize) const = 0;
    virtual std::vector<Position> GetRelatedVerticalPositions(Position const& selectedPosition, int gridSize) const = 0;
    virtual std::vector<Position> GetRelatedBlockPositions(Position const& selectedPosition, int gridSize, int blockSize) const = 0;
    virtual std::vector<Position> GetAllRelatedPositions(Position const& selectedPosition, int gridSize, int blockSize) const = 0;
};

class RelatedPositionsGetterImpl : public RelatedPositionsGetter
{
public:
    std::vector<Position> GetRelatedHorizontalPositions(Position const& selectedPosition, int gridSize) const override;
    std::vector<Position> GetRelatedVerticalPositions(Position const& selectedPosition, int gridSize) const override;
    std::vector<Position> GetRelatedBlockPositions(Position const& selectedPosition, int gridSize, int blockSize) const override;
    std::vector<Position> GetAllRelatedPositions(Position const& selectedPosition, int gridSize, int blockSize) const override;
};

} /* namespace sudoku */

