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

inline Grid Create4x4CorrectlySolvedGrid()
{
    Grid grid(4);

    grid.m_Cells[0][0]->SetValue(1);
    grid.m_Cells[0][1]->SetValue(2);
    grid.m_Cells[0][2]->SetValue(3);
    grid.m_Cells[0][3]->SetValue(4);
    grid.m_Cells[1][0]->SetValue(3);
    grid.m_Cells[1][1]->SetValue(4);
    grid.m_Cells[1][2]->SetValue(1);
    grid.m_Cells[1][3]->SetValue(2);
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

inline Grid Create4x4CorrectlyPartiallyFilledGrid()
{
    Grid grid(4);

    grid.m_Cells[0][0]->SetValue(1);
    grid.m_Cells[0][2]->SetValue(3);
    grid.m_Cells[1][0]->SetValue(3);
    grid.m_Cells[1][3]->SetValue(2);
    grid.m_Cells[2][1]->SetValue(3);
    grid.m_Cells[3][0]->SetValue(4);

    return grid;
}

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

} /* namespace test */
} /* namespace sudoku */
