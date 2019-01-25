#include <unordered_map>
#include <vector>
#include <numeric>

#include <boost/range/algorithm_ext.hpp>

#include "Cell.hpp"
#include "Value.hpp"

namespace sudoku
{
namespace test
{

inline void RemoveAllCellPossibilitiesBut(Cell& cell, std::unordered_set<Value> const& possibilitiesToKeep)
{
    std::vector<Value> possibilitiesToRemove(9);
    std::iota(possibilitiesToRemove.begin(), possibilitiesToRemove.end(), 1);

    if (!possibilitiesToKeep.empty())
        boost::range::remove_erase_if(possibilitiesToRemove, [&](auto v){ return possibilitiesToKeep.count(v); });

    for (auto possibility : possibilitiesToRemove)
        cell.RemovePossibility(possibility);
}

} /* namespace test */
} /* namespace sudoku */
