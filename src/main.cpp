#include <iostream>

#include <chrono>

#include <boost/range/irange.hpp>
#include <boost/range/algorithm.hpp>

#include "GridSolverFactory.hpp"
#include "GridStatus.hpp"
#include "Grid.hpp"
#include "Position.hpp"
#include "Value.hpp"
#include "utils/Utils.hpp"

using namespace sudoku;
using namespace sudoku::test;

// Benchmark for solving 9x9 Sudoku with 20 cells set at the beginning

// Optimisations steps:
// 1) Improvement in the way the detection of wrong new cells was made.
// 2) Optimisation of Possibilities class to use bitset instead of unordered_set
// 3) Utilisation of thread pool instead of always creating/deleting new threads
// 4) Grid stores Cells instead of shared_ptr of Cells to avoid indirection
// 5) Stop using multithreading because the overhead is too big
// 6) Get related position at compile time
// 6) Create lookup table creating at compile time for number possibility left

// Execution time
// Before any optimisation: 28'857 micro seconds
// optimisation 1: 11'970 micro seconds
// optimisation 2: 5'706 micro seconds
// optimisation 3: 2'648 micro seconds
// optimisation 4: 1'935 micro seconds
// optimisation 5: 754 micro seconds
// optimisation 6: 441 micro seconds
// optimisation 7: 313 micro seconds

int GetMedian(std::vector<int> v)
{
    const auto middle = v.size() / 2;
    std::nth_element(v.begin(), v.begin() + middle, v.end());
    return v[middle];
}

int main ()
{
    std::cout << "Multithreaded Sudoku Solver" << std::endl;

    const int gridSize {9};
    const int cellsKept {20};

    const auto positionsValues = CreatePositionsValues9x9();

    auto gridSolver = GridSolverFactory::Make();

    std::vector<int> durations;

    const int testExecutionCount = 2'000;
    for([[gnu::unused]] int i : boost::irange(0, testExecutionCount))
    {
        auto grid = CreateGrid(gridSize, KeepRandomCells(positionsValues, cellsKept));

        try
        {
            const auto beg = std::chrono::high_resolution_clock::now();
            const auto solvedCorrectly = gridSolver->Solve(grid);
            const auto end = std::chrono::high_resolution_clock::now();

            if (!solvedCorrectly)
                throw std::runtime_error("Grid solved incorrectly by solver");

            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - beg).count();
            durations.push_back(duration);
        }
        catch(std::exception& e)
        {
            std::cout << "Couldn't solve grid " << std::endl << grid << std::endl << " because: " << e.what() << std::endl;
        }
    }

    std::cout << "number of execution: " << durations.size() << std::endl;

    const auto median = GetMedian(durations);

    std::vector<int> absDeviations(durations.size());
    boost::transform(durations, absDeviations.begin(), [median](int val){ return std::abs(val - median); });

    const auto mad = GetMedian(std::move(absDeviations));

    std::cout << "median duration: " << median << " micro seconds" << std::endl;
    std::cout << "median absolute deviation duration: " << mad << " micro seconds" << std::endl;

    return 0;
}
