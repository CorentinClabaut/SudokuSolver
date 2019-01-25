#pragma once

#include "ostream"

namespace sudoku
{

struct Position
{
    int m_Row;
    int m_Col;
};

inline bool operator==(Position const& lhs, Position const& rhs)
{
    return std::tie(lhs.m_Col, lhs.m_Row)
        == std::tie(rhs.m_Col, rhs.m_Row);
}

inline bool operator<(Position const& lhs, Position const& rhs)
{
    return lhs.m_Row == rhs.m_Row ?
                lhs.m_Col < rhs.m_Col :
                lhs.m_Row < rhs.m_Row;

}

inline std::ostream& operator<<(std::ostream& os, Position const& pos)
{
    return os
            << "Row [" << pos.m_Row << "] "
            << "Col [" << pos.m_Col << "]";
}

} // namespace sudoku
