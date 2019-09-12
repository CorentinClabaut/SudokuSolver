#include "RelatedPositionsGetter.hpp"

#include "Position.hpp"

using namespace sudoku;

template<int TPositionsCount, int TGroupsCount>
constexpr std::array<Range<Position>, TGroupsCount> CreateArrayOfRanges(std::array<std::array<Position, TPositionsCount>, TGroupsCount> const& allGroupsPositions)
{
    std::array<Range<Position>, TGroupsCount> ranges {};

    auto it = ranges.begin();

    for (auto const& group : allGroupsPositions)
    {
        *it = Range<Position> {group};
        it++;
    }

    return ranges;
}

template<int TGridSize, int TGroupSize>
struct Ranges
{
    constexpr Ranges(std::array<std::array<Position, TGridSize>, TGroupSize> allGroupsPositions) :
        m_AllGroupsPositions(allGroupsPositions),
        m_ArrayOfRanges(CreateArrayOfRanges<TGridSize, TGroupSize>(m_AllGroupsPositions)),
        m_Ranges(m_ArrayOfRanges)
    {}

    constexpr Range<Position> const& operator[](int i) const { return m_Ranges[i]; }

    std::array<std::array<Position, TGridSize>, TGroupSize> m_AllGroupsPositions;
    std::array<Range<Position>, TGroupSize> m_ArrayOfRanges;
    Range<Range<Position>> m_Ranges;
};


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
constexpr std::array<Position, TGridSize> GetAllPositions(
        int begRow,
        int rows,
        int begCol,
        int cols)
{
    std::array<Position, TGridSize> related {};
    auto relatedIter = related.begin();

    for(int row = begRow; row < begRow + rows; row++)
    {
        for(int col = begCol; col < begCol + cols; col++)
        {
            *relatedIter = Position {row, col};
            relatedIter++;
        }
    }

    return related;
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

    for (auto const& pos : GetAllPositions<TGridSize>(begRow, rows, begCol, cols))
    {
        if (pos == currentPosition)
            continue;

        *relatedIter = pos;
        relatedIter++;
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

template<int TGridSize>
constexpr std::array<Position, GetAllRelatedPositionNumber(TGridSize)> CreateAllRelatedPositions(Position const& currentPosition)
{
    std::array<Position, GetAllRelatedPositionNumber(TGridSize)> allRelatedWithoutDuplication {};

    auto it = allRelatedWithoutDuplication.begin();

    for (auto const& relatedPositions : {CreateVerticalRelatedPositions<TGridSize>(currentPosition), CreateHorizontalRelatedPositions<TGridSize>(currentPosition), CreateBlockRelatedPositions<TGridSize>(currentPosition)})
    {
        for (auto const& pos : relatedPositions)
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

constexpr int PosToInt(Position const& position, int gridSize)
{
    return position.m_Row * gridSize + position.m_Col;
}

template<int TGridSize, int TRelatedPosCount, typename Fun>
constexpr std::array<std::array<Position, TRelatedPosCount>, TGridSize * TGridSize> CreateAllRelatedPositionsGroups(Fun GetRelatedPosition)
{
    std::array<std::array<Position, TRelatedPosCount>, TGridSize * TGridSize> allRelatedPositionsGroups {};

    for(int row = 0; row < TGridSize; row++)
    {
        for(int col = 0; col < TGridSize; col++)
        {
            Position pos {row, col};

            allRelatedPositionsGroups[PosToInt(pos, TGridSize)] = GetRelatedPosition(pos);
        }
    }

    return allRelatedPositionsGroups;
}

template<int TGridSize>
struct AllRelatedPositionsGroups
{
    Ranges<TGridSize - 1, TGridSize * TGridSize> m_Vertical = CreateAllRelatedPositionsGroups<TGridSize, TGridSize - 1>(CreateVerticalRelatedPositions<TGridSize>);
    Ranges<TGridSize - 1, TGridSize * TGridSize> m_Horizontal = CreateAllRelatedPositionsGroups<TGridSize, TGridSize - 1>(CreateHorizontalRelatedPositions<TGridSize>);
    Ranges<TGridSize - 1, TGridSize * TGridSize> m_Block = CreateAllRelatedPositionsGroups<TGridSize, TGridSize - 1>(CreateBlockRelatedPositions<TGridSize>);

    Ranges<GetAllRelatedPositionNumber(TGridSize), TGridSize * TGridSize> m_All = CreateAllRelatedPositionsGroups<TGridSize, GetAllRelatedPositionNumber(TGridSize)>(CreateAllRelatedPositions<TGridSize>);
};

constexpr AllRelatedPositionsGroups<9> AllRelatedPositionsGroups9x9 {};
constexpr AllRelatedPositionsGroups<4> AllRelatedPositionsGroups4x4 {};

constexpr int GetGroupsNumberInGrid(int gridSize)
{
    constexpr int groupType {3};
    return gridSize * groupType;
}

template<int TGridSize>
constexpr std::array<std::array<Position, TGridSize>, GetGroupsNumberInGrid(TGridSize)> CreateAllGroupsPositions()
{
    std::array<std::array<Position, TGridSize>, GetGroupsNumberInGrid(TGridSize)> allGroupsPositions {};

    auto it = allGroupsPositions.begin();

    for (int col = 0; col < TGridSize; col++)
    {
        *it = GetAllPositions<TGridSize>(0, TGridSize, col, 1);
        it++;
    }

    for (int row = 0; row < TGridSize; row++)
    {
        *it = GetAllPositions<TGridSize>(row, 1, 0, TGridSize);
        it++;
    }

    const int blockSize = GetBlockSize(TGridSize);
    for (int row = 0; row < TGridSize; row += blockSize)
    {
        for (int col = 0; col < TGridSize; col += blockSize)
        {
            *it = GetAllPositions<TGridSize>(row, blockSize, col, blockSize);
            it++;
        }
    }

    return allGroupsPositions;
}

template<int TGridSize>
struct AllGroupsPositions
{
    Ranges<TGridSize, GetGroupsNumberInGrid(TGridSize)> m_Ranges = CreateAllGroupsPositions<TGridSize>();
};

constexpr AllGroupsPositions<9> AllGroupsPositions9x9 {};
constexpr AllGroupsPositions<4> AllGroupsPositions4x4 {};

Range<Position> RelatedPositionsGetterImpl::GetRelatedHorizontalPositions(Position const& selectedPosition, int gridSize) const
{
    return gridSize == 4 ?
                AllRelatedPositionsGroups4x4.m_Horizontal[PosToInt(selectedPosition, gridSize)] :
                AllRelatedPositionsGroups9x9.m_Horizontal[PosToInt(selectedPosition, gridSize)];
}

Range<Position> RelatedPositionsGetterImpl::GetRelatedVerticalPositions(Position const& selectedPosition, int gridSize) const
{
    return gridSize == 4 ?
                AllRelatedPositionsGroups4x4.m_Vertical[PosToInt(selectedPosition, gridSize)] :
                AllRelatedPositionsGroups9x9.m_Vertical[PosToInt(selectedPosition, gridSize)];
}

Range<Position> RelatedPositionsGetterImpl::GetRelatedBlockPositions(Position const& selectedPosition, int gridSize) const
{
    return gridSize == 4 ?
                AllRelatedPositionsGroups4x4.m_Block[PosToInt(selectedPosition, gridSize)] :
                AllRelatedPositionsGroups9x9.m_Block[PosToInt(selectedPosition, gridSize)];
}

Range<Position> RelatedPositionsGetterImpl::GetAllRelatedPositions(Position const& selectedPosition, int gridSize) const
{
    return gridSize == 4 ?
                AllRelatedPositionsGroups4x4.m_All[PosToInt(selectedPosition, gridSize)] :
                AllRelatedPositionsGroups9x9.m_All[PosToInt(selectedPosition, gridSize)];
}

Range<Range<Position>> RelatedPositionsGetterImpl::GetAllGroupsPositions(int gridSize) const
{
    return gridSize == 4 ? AllGroupsPositions4x4.m_Ranges.m_Ranges : AllGroupsPositions9x9.m_Ranges.m_Ranges;
}
