#pragma once

#include "Cell.hpp"

namespace sudoku
{

struct FoundCells;

class FoundCellsEnqueuer
{
public:
    virtual ~FoundCellsEnqueuer() = default;

    virtual void Enqueue(SharedCell cell) = 0;
};

class FoundCellsEnqueuerImpl : public FoundCellsEnqueuer
{
public:
    FoundCellsEnqueuerImpl(std::shared_ptr<FoundCells> foundCells);

    void Enqueue(SharedCell cell) override;

private:
    std::shared_ptr<FoundCells> m_FoundCells;
};

} /* namespace sudoku */

