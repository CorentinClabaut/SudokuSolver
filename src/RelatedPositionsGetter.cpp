#include "RelatedPositionsGetter.hpp"

#include "Position.hpp"

using namespace sudoku;

constexpr int isqrt_impl(int sq, int dlt, int value)
{
    return sq <= value ? isqrt_impl(sq+dlt, dlt+2, value) : (dlt >> 1) - 1;
}

constexpr int isqrt(int value)
{
    return isqrt_impl(1, 3, value);
}

constexpr int GetBlockSize(int gridSize)
{
    return isqrt(gridSize);
}

constexpr int GetAllRelatedPositionNumber(int gridSize)
{
    const auto blockSize = GetBlockSize(gridSize);

    return (3 * gridSize - 3) - (2 * blockSize - 2);
}

constexpr int GetNumberOfRelatedPositionInGroup(int gridSize)
{
    return gridSize - 1;
}

template<int TSize>
using ArrayRelatedPosition = std::array<Position, TSize>;

template<class Range>
constexpr bool any_of_equal(Range range, typename Range::value_type comp)
{
    for (auto val : range)
    {
        if (val == comp)
            return true;
    }

    return false;
}

constexpr int RoundDown(int value, int multiplier)
{
    return (value / multiplier) * multiplier;
}

template<int TGridSize>
constexpr std::array<Position, GetNumberOfRelatedPositionInGroup(TGridSize)> GetAllPositionsButSelectedOne(
        int begRow,
        int rows,
        int begCol,
        int cols,
        Position const& currentPosition)
{
    std::array<Position, GetNumberOfRelatedPositionInGroup(TGridSize)> related {};
    auto relatedIter = related.begin();

    for(int row = begRow; row < begRow + rows; row++)
    {
        for(int col = begCol; col < begCol + cols; col++)
        {
            Position pos {row, col};

            if (pos == currentPosition)
                continue;

            *relatedIter = pos;

            relatedIter++;
        }
    }

    return related;
}

template<int TGridSize>
constexpr std::array<Position, GetNumberOfRelatedPositionInGroup(TGridSize)> CreateVerticalRelatedPositions(Position const& currentPosition)
{
    return GetAllPositionsButSelectedOne<TGridSize>(0, TGridSize, currentPosition.m_Col, 1, currentPosition);
}

template<int TGridSize>
constexpr std::array<Position, GetNumberOfRelatedPositionInGroup(TGridSize)> CreateHorizontalRelatedPositions(Position const& currentPosition)
{
    return GetAllPositionsButSelectedOne<TGridSize>(currentPosition.m_Row, 1, 0, TGridSize, currentPosition);
}

template<int TGridSize>
constexpr std::array<Position, GetNumberOfRelatedPositionInGroup(TGridSize)> CreateBlockRelatedPositions(Position const& currentPosition)
{
    auto blockSize = GetBlockSize(TGridSize);

    auto beginRow = RoundDown(currentPosition.m_Row, blockSize);
    auto beginCol = RoundDown(currentPosition.m_Col, blockSize);
    return GetAllPositionsButSelectedOne<TGridSize>(beginRow, blockSize, beginCol, blockSize, currentPosition);
}

constexpr int RelatedPositionTypeCount = 3;

template<int TGridSize>
constexpr std::array<Position, GetAllRelatedPositionNumber(TGridSize)> CreateAllRelatedPositions(
        const std::array<std::array<Position, GetNumberOfRelatedPositionInGroup(TGridSize)>, RelatedPositionTypeCount>& allRelatedPositions)
{
    std::array<Position, GetAllRelatedPositionNumber(TGridSize)> allRelatedWithoutDuplication {};

    auto it = allRelatedWithoutDuplication.begin();

    for (const auto& relatedPositions : allRelatedPositions)
    {
        for (const auto& pos : relatedPositions)
        {
            if (!any_of_equal(allRelatedWithoutDuplication, pos))
            {
                *it = pos;
                it++;
            }
        }
    }

    return allRelatedWithoutDuplication;
}

template<int TGridSize>
struct RelatedPositionsGroups
{
    constexpr RelatedPositionsGroups() = default;

    constexpr RelatedPositionsGroups(Position currentPosition) :
        m_Horizontal(CreateHorizontalRelatedPositions<TGridSize>(currentPosition)),
        m_Vertical(CreateVerticalRelatedPositions<TGridSize>(currentPosition)),
        m_Block(CreateBlockRelatedPositions<TGridSize>(currentPosition)),
        m_All(CreateAllRelatedPositions<TGridSize>({m_Horizontal, m_Vertical, m_Block}))
    {}

    std::array<Position, GetNumberOfRelatedPositionInGroup(TGridSize)> m_Horizontal;
    std::array<Position, GetNumberOfRelatedPositionInGroup(TGridSize)> m_Vertical;
    std::array<Position, GetNumberOfRelatedPositionInGroup(TGridSize)> m_Block;

    std::array<Position, GetAllRelatedPositionNumber(TGridSize)> m_All;
};

constexpr int PosToInt(Position const& position, int gridSize)
{
    return position.m_Row * gridSize + position.m_Col;
}

template<int TGridSize>
constexpr std::array<RelatedPositionsGroups<TGridSize>, TGridSize * TGridSize> CreateAllRelatedPositionsGroups()
{
    std::array<RelatedPositionsGroups<TGridSize>, TGridSize * TGridSize> allRelatedPositionsGroups {};

    for(int row = 0; row < TGridSize; row++)
    {
        for(int col = 0; col < TGridSize; col++)
        {
            Position pos {row, col};

            RelatedPositionsGroups<TGridSize> relatedPositionsGroups {pos};
            allRelatedPositionsGroups[PosToInt(pos, TGridSize)] = relatedPositionsGroups;
        }
    }

    return allRelatedPositionsGroups;
}

template<int TGridSize>
struct AllRelatedPositionsGroups
{
    constexpr AllRelatedPositionsGroups() : m_AllRelatedPositionsGroups(CreateAllRelatedPositionsGroups<TGridSize>())
    {}

    constexpr RelatedPositionsGroups<TGridSize> const& operator[](Position const& position) const
    {
        return m_AllRelatedPositionsGroups[PosToInt(position, TGridSize)];
    }

    std::array<RelatedPositionsGroups<TGridSize>, TGridSize * TGridSize> m_AllRelatedPositionsGroups;
};

constexpr AllRelatedPositionsGroups<9> AllRelatedPositionsGroups9x9 {};
constexpr AllRelatedPositionsGroups<4> AllRelatedPositionsGroups4x4 {};

Range<Position> RelatedPositionsGetterImpl::GetRelatedHorizontalPositions(Position const& selectedPosition, int gridSize) const
{
    return gridSize == 4 ?
                Range<Position>{ AllRelatedPositionsGroups4x4[selectedPosition].m_Horizontal } :
                Range<Position>{ AllRelatedPositionsGroups9x9[selectedPosition].m_Horizontal };
}

Range<Position> RelatedPositionsGetterImpl::GetRelatedVerticalPositions(Position const& selectedPosition, int gridSize) const
{
    return gridSize == 4 ?
                Range<Position>{ AllRelatedPositionsGroups4x4[selectedPosition].m_Vertical } :
                Range<Position>{ AllRelatedPositionsGroups9x9[selectedPosition].m_Vertical };
}

Range<Position> RelatedPositionsGetterImpl::GetRelatedBlockPositions(Position const& selectedPosition, int gridSize) const
{
    return gridSize == 4 ?
                Range<Position>{ AllRelatedPositionsGroups4x4[selectedPosition].m_Block } :
                Range<Position>{ AllRelatedPositionsGroups9x9[selectedPosition].m_Block };
}

Range<Position> RelatedPositionsGetterImpl::GetAllRelatedPositions(Position const& selectedPosition, int gridSize) const
{
    return gridSize == 4 ?
                Range<Position>{ AllRelatedPositionsGroups4x4[selectedPosition].m_All } :
                Range<Position>{ AllRelatedPositionsGroups9x9[selectedPosition].m_All };
}
