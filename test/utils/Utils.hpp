#include <unordered_map>
#include <vector>
#include <numeric>
#include <atomic>
#include <thread>

#include <boost/range/algorithm_ext.hpp>

#include "Cell.hpp"
#include "Value.hpp"
#include "Grid.hpp"

namespace sudoku
{
namespace test
{

using PositionsValues = std::vector<std::pair<Position, Value>>;

inline void RemoveAllCellPossibilitiesBut(Cell& cell, std::unordered_set<Value> const& possibilitiesToKeep)
{
    std::vector<Value> possibilitiesToRemove(9);
    std::iota(possibilitiesToRemove.begin(), possibilitiesToRemove.end(), 1);

    if (!possibilitiesToKeep.empty())
        boost::range::remove_erase_if(possibilitiesToRemove, [&](auto v){ return possibilitiesToKeep.count(v); });

    for (auto possibility : possibilitiesToRemove)
        cell.RemovePossibility(possibility);
}

class ParallelThreadsCounter
{
public:
    void count()
    {
        threadsWorking++;

        int max = std::max(threadsWorking, maxThreadsWorkingInParallel);
        maxThreadsWorkingInParallel = max;

        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        threadsWorking--;
    }

    int GetMaxThreadsWorkingInParallel()
    {
        return maxThreadsWorkingInParallel;
    }

private:
    std::atomic<int> threadsWorking {0};
    std::atomic<int> maxThreadsWorkingInParallel {0};
};

inline Grid Create4x4IncorrectlySolvedGrid()
{
    Grid grid(4);

    grid.m_Cells[0][0]->SetValue(1);
    grid.m_Cells[0][1]->SetValue(2);
    grid.m_Cells[0][2]->SetValue(3);
    grid.m_Cells[0][3]->SetValue(4);
    grid.m_Cells[1][0]->SetValue(3);
    grid.m_Cells[1][1]->SetValue(4);
    grid.m_Cells[1][2]->SetValue(1);
    grid.m_Cells[1][3]->SetValue(4); // wrong value, should be 2
    grid.m_Cells[2][0]->SetValue(2);
    grid.m_Cells[2][1]->SetValue(3);
    grid.m_Cells[2][2]->SetValue(4);
    grid.m_Cells[2][3]->SetValue(1);
    grid.m_Cells[3][0]->SetValue(4);
    grid.m_Cells[3][1]->SetValue(1);
    grid.m_Cells[3][2]->SetValue(2);
    grid.m_Cells[3][3]->SetValue(3);

    return grid;
}


inline PositionsValues CreatePositionsValues9x9()
{
    return {
        {Position{0, 0}, Value{6}},
        {Position{0, 1}, Value{3}},
        {Position{0, 2}, Value{9}},
        {Position{0, 3}, Value{5}},
        {Position{0, 4}, Value{7}},
        {Position{0, 5}, Value{4}},
        {Position{0, 6}, Value{1}},
        {Position{0, 7}, Value{8}},
        {Position{0, 8}, Value{2}},

        {Position{1, 0}, Value{5}},
        {Position{1, 1}, Value{4}},
        {Position{1, 2}, Value{1}},
        {Position{1, 3}, Value{8}},
        {Position{1, 4}, Value{2}},
        {Position{1, 5}, Value{9}},
        {Position{1, 6}, Value{3}},
        {Position{1, 7}, Value{7}},
        {Position{1, 8}, Value{6}},

        {Position{2, 0}, Value{7}},
        {Position{2, 1}, Value{8}},
        {Position{2, 2}, Value{2}},
        {Position{2, 3}, Value{6}},
        {Position{2, 4}, Value{1}},
        {Position{2, 5}, Value{3}},
        {Position{2, 6}, Value{9}},
        {Position{2, 7}, Value{5}},
        {Position{2, 8}, Value{4}},

        {Position{3, 0}, Value{1}},
        {Position{3, 1}, Value{9}},
        {Position{3, 2}, Value{8}},
        {Position{3, 3}, Value{4}},
        {Position{3, 4}, Value{6}},
        {Position{3, 5}, Value{7}},
        {Position{3, 6}, Value{5}},
        {Position{3, 7}, Value{2}},
        {Position{3, 8}, Value{3}},

        {Position{4, 0}, Value{3}},
        {Position{4, 1}, Value{6}},
        {Position{4, 2}, Value{5}},
        {Position{4, 3}, Value{9}},
        {Position{4, 4}, Value{8}},
        {Position{4, 5}, Value{2}},
        {Position{4, 6}, Value{4}},
        {Position{4, 7}, Value{1}},
        {Position{4, 8}, Value{7}},

        {Position{5, 0}, Value{4}},
        {Position{5, 1}, Value{2}},
        {Position{5, 2}, Value{7}},
        {Position{5, 3}, Value{1}},
        {Position{5, 4}, Value{3}},
        {Position{5, 5}, Value{5}},
        {Position{5, 6}, Value{8}},
        {Position{5, 7}, Value{6}},
        {Position{5, 8}, Value{9}},

        {Position{6, 0}, Value{9}},
        {Position{6, 1}, Value{5}},
        {Position{6, 2}, Value{6}},
        {Position{6, 3}, Value{7}},
        {Position{6, 4}, Value{4}},
        {Position{6, 5}, Value{8}},
        {Position{6, 6}, Value{2}},
        {Position{6, 7}, Value{3}},
        {Position{6, 8}, Value{1}},

        {Position{7, 0}, Value{8}},
        {Position{7, 1}, Value{1}},
        {Position{7, 2}, Value{3}},
        {Position{7, 3}, Value{2}},
        {Position{7, 4}, Value{9}},
        {Position{7, 5}, Value{6}},
        {Position{7, 6}, Value{7}},
        {Position{7, 7}, Value{4}},
        {Position{7, 8}, Value{5}},

        {Position{8, 0}, Value{2}},
        {Position{8, 1}, Value{7}},
        {Position{8, 2}, Value{4}},
        {Position{8, 3}, Value{3}},
        {Position{8, 4}, Value{5}},
        {Position{8, 5}, Value{1}},
        {Position{8, 6}, Value{6}},
        {Position{8, 7}, Value{9}},
        {Position{8, 8}, Value{8}},
    };
}

inline PositionsValues CreatePositionsValues4x4()
{
    return {
        {Position{0, 0}, Value{1}},
        {Position{0, 1}, Value{2}},
        {Position{0, 2}, Value{3}},
        {Position{0, 3}, Value{4}},
        {Position{1, 0}, Value{3}},
        {Position{1, 1}, Value{4}},
        {Position{1, 2}, Value{1}},
        {Position{1, 3}, Value{2}},
        {Position{2, 0}, Value{2}},
        {Position{2, 1}, Value{3}},
        {Position{2, 2}, Value{4}},
        {Position{2, 3}, Value{1}},
        {Position{3, 0}, Value{4}},
        {Position{3, 1}, Value{1}},
        {Position{3, 2}, Value{2}},
        {Position{3, 3}, Value{3}},
    };
}

inline PositionsValues KeepRandomCells(PositionsValues positionsValues, int cellKeptCount)
{
    std::random_shuffle(positionsValues.begin(), positionsValues.end());
    positionsValues.resize(cellKeptCount);

    return positionsValues;
}

inline Grid CreateGrid(int gridSize, PositionsValues const& positionsValues)
{
    Grid grid {gridSize};

    for (auto & [position, value] : positionsValues)
    {
        grid.m_Cells[position.m_Row][position.m_Col]->SetValue(value);
    }

    return grid;
}

inline Grid Create4x4CorrectlySolvedGrid()
{
    auto positionsValues = CreatePositionsValues4x4();
    return CreateGrid(4, positionsValues);
}

inline Grid Create4x4CorrectlyPartiallyFilledGrid()
{
    auto positionsValues = CreatePositionsValues4x4();
    KeepRandomCells(positionsValues, 6);
    return CreateGrid(4, positionsValues);
}

} /* namespace test */
} /* namespace sudoku */
