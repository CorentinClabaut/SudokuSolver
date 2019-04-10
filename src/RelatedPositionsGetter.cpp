#include "RelatedPositionsGetter.hpp"

#include "Position.hpp"

#include <boost/range/irange.hpp>

using namespace sudoku;

namespace
{
int RoundDown(int value, int multiplier)
{
    return (value / multiplier) * multiplier;
}

void GetAllPositionsButSelectedOne(int begRow, int rows, int begCol, int cols, Position const& selectedPosition, std::vector<Position>& positions)
{
    for(auto row : boost::irange(begRow, begRow + rows))
    {
        for(auto col : boost::irange(begCol, begCol + cols))
        {
            Position pos {row, col};

            if (pos == selectedPosition)
                continue;

            positions.push_back(std::move(pos));
        }
    }
}

void GetRelatedHorizontalPositionsImpl(Position const& selectedPosition, int gridSize, std::vector<Position>& positions)
{
    GetAllPositionsButSelectedOne(selectedPosition.m_Row, 1, 0, gridSize, selectedPosition, positions);
}

void GetRelatedVerticalPositionsImpl(Position const& selectedPosition, int gridSize, std::vector<Position>& positions)
{
    GetAllPositionsButSelectedOne(0, gridSize, selectedPosition.m_Col, 1, selectedPosition, positions);
}

void GetRelatedBlockPositionsImpl(Position const& selectedPosition, int blockSize, std::vector<Position>& positions)
{
    auto beginRow = RoundDown(selectedPosition.m_Row, blockSize);
    auto beginCol = RoundDown(selectedPosition.m_Col, blockSize);

    GetAllPositionsButSelectedOne(beginRow, blockSize, beginCol, blockSize, selectedPosition, positions);
}

void GetAllRelatedPositionsImpl(Position const& selectedPosition, int gridSize, int blockSize, std::vector<Position>& positions)
{
    GetRelatedHorizontalPositionsImpl(selectedPosition, gridSize, positions);
    GetRelatedVerticalPositionsImpl(selectedPosition, gridSize, positions);
    GetRelatedBlockPositionsImpl(selectedPosition, blockSize, positions);

    std::sort(positions.begin(), positions.end());
    auto end = std::unique(positions.begin(), positions.end());
    positions.erase(end, positions.end());
}
} // anonymous namespace

std::vector<Position> RelatedPositionsGetterImpl::GetRelatedHorizontalPositions(Position const& selectedPosition, int gridSize) const
{
    std::vector<Position> positions;
    positions.reserve(gridSize);

    GetRelatedHorizontalPositionsImpl(selectedPosition, gridSize, positions);

    return positions;
}

std::vector<Position> RelatedPositionsGetterImpl::GetRelatedVerticalPositions(Position const& selectedPosition, int gridSize) const
{
    std::vector<Position> positions;
    positions.reserve(gridSize);

    GetRelatedVerticalPositionsImpl(selectedPosition, gridSize, positions);

    return positions;
}

std::vector<Position> RelatedPositionsGetterImpl::GetRelatedBlockPositions(Position const& selectedPosition, int gridSize, int blockSize) const
{
    std::vector<Position> positions;
    positions.reserve(gridSize);

    GetRelatedBlockPositionsImpl(selectedPosition, blockSize, positions);

    return positions;
}

std::vector<Position> RelatedPositionsGetterImpl::GetAllRelatedPositions(Position const& selectedPosition, int gridSize, int blockSize) const
{
    std::vector<Position> positions;
    positions.reserve(gridSize * 3);

    GetAllRelatedPositionsImpl(selectedPosition, gridSize, blockSize, positions);

    return positions;
}
