#pragma once

#include <ostream>

namespace sudoku
{

enum class GridStatus
{
    SolvedCorrectly,
    Wrong,
    Incomplete
};

inline std::ostream& operator<<(std::ostream& os, GridStatus gridStatus)
{
    switch (gridStatus)
    {
    case GridStatus::SolvedCorrectly : return os << "SolvedCorrectly";
    case GridStatus::Wrong : return os << "Wrong";
    case GridStatus::Incomplete : return os << "Incomplete";
    }

    throw std::runtime_error("Can't display gridStatus value '" + std::to_string(static_cast<int>(gridStatus)) + "'");
}

} /* namespace sudoku */

