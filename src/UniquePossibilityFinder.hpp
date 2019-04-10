#pragma once

#include <optional>
#include <memory>

#include "Value.hpp"

namespace sudoku
{

class RelatedPositionsGetter;
struct Position;
struct Grid;

class UniquePossibilityFinder
{
public:
    virtual ~UniquePossibilityFinder() = default;

    virtual std::optional<Value> FindUniquePossibility(Position const& cellPosition, Grid& grid) const = 0;
};

class UniquePossibilityFinderImpl : public UniquePossibilityFinder
{
public:
    UniquePossibilityFinderImpl(std::unique_ptr<RelatedPositionsGetter> relatedPositionsGetter);

    std::optional<Value> FindUniquePossibility(Position const& cellPosition, Grid& grid) const override;

private:
    std::unique_ptr<RelatedPositionsGetter> m_RelatedPositionsGetter;
};

} /* namespace sudoku */

