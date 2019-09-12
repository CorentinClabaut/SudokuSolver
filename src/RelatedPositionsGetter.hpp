#pragma once

#include "Position.hpp"

namespace sudoku
{

template<typename T>
struct Range
{
    template<size_t TSize>
    constexpr Range(std::array<T, TSize> const& a) :
        begin_(a.begin()),
        end_(a.end())
    {}

    constexpr Range() :
        begin_(nullptr),
        end_(nullptr)
    {}

    constexpr T const& operator[](int i) const { return *(begin_ + i); }

    constexpr int size() const { return end_ - begin_; }

    constexpr T const* begin() const { return begin_; }
    constexpr T const* end() const { return end_; }

    T const* begin_;
    T const* end_;
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

    virtual Range<Range<Position>> GetAllGroupsPositions(int gridSize) const = 0;
};

class RelatedPositionsGetterImpl : public RelatedPositionsGetter
{
public:
    Range<Position> GetRelatedHorizontalPositions(Position const& selectedPosition, int gridSize) const override;
    Range<Position> GetRelatedVerticalPositions(Position const& selectedPosition, int gridSize) const override;
    Range<Position> GetRelatedBlockPositions(Position const& selectedPosition, int gridSize) const override;
    Range<Position> GetAllRelatedPositions(Position const& selectedPosition, int gridSize) const override;

    Range<Range<Position>> GetAllGroupsPositions(int gridSize) const override;
};

} /* namespace sudoku */

