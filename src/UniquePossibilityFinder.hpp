#pragma once

#include <optional>
#include <memory>

#include "Value.hpp"

namespace sudoku
{

class RelatedCellsGetter;
struct Position;
struct Grid;

class UniquePossibilityFinder
{
public:
    virtual ~UniquePossibilityFinder() = default;

    virtual std::optional<Value> FindUniquePossibility(Position const& cellPosition, Grid const& grid) const = 0;
};

class UniquePossibilityFinderImpl : public UniquePossibilityFinder
{
public:
    UniquePossibilityFinderImpl(std::unique_ptr<RelatedCellsGetter> relatedCellsGetter);

    std::optional<Value> FindUniquePossibility(Position const& cellPosition, Grid const& grid) const override;

private:
    std::unique_ptr<RelatedCellsGetter> m_RelatedCellsGetter;
};

} /* namespace sudoku */

