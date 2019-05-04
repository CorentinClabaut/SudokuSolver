#pragma once

#include "Position.hpp"

namespace sudoku
{

template<typename T>
struct Range
{
    template<size_t TSize>
    Range(const std::array<T, TSize>& a) :
        begin_(a.begin()),
        end_(a.end())
    {}

    int size() const { return std::distance(begin_, end_); }

    T const* begin() const { return begin_; }
    T const* end() const { return end_; }

    T const* const begin_;
    T const* const end_;
};

class Position;

class RelatedPositionsGetter
{
public:
    virtual ~RelatedPositionsGetter() = default;

    virtual Range<Position> GetRelatedHorizontalPositions(Position const& selectedPosition, int gridSize) const = 0;
    virtual Range<Position> GetRelatedVerticalPositions(Position const& selectedPosition, int gridSize) const = 0;
    virtual Range<Position> GetRelatedBlockPositions(Position const& selectedPosition, int gridSize) const = 0;
    virtual Range<Position> GetAllRelatedPositions(Position const& selectedPosition, int gridSize) const = 0;
};

class RelatedPositionsGetterImpl : public RelatedPositionsGetter
{
public:
    Range<Position> GetRelatedHorizontalPositions(Position const& selectedPosition, int gridSize) const override;
    Range<Position> GetRelatedVerticalPositions(Position const& selectedPosition, int gridSize) const override;
    Range<Position> GetRelatedBlockPositions(Position const& selectedPosition, int gridSize) const override;
    Range<Position> GetAllRelatedPositions(Position const& selectedPosition, int gridSize) const override;
};

} /* namespace sudoku */

