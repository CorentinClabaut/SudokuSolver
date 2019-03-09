#include <iostream>

#include <chrono>
#include <thread>

#include <boost/range/numeric.hpp>

#include "GridSolverFactory.hpp"
#include "GridStatus.hpp"
#include "Grid.hpp"
#include "Position.hpp"
#include "Value.hpp"
#include "utils/Utils.hpp"

using namespace sudoku;
using namespace sudoku::test;

int main ()
{
    std::cout << "Multithreaded Sudoku Solver" << std::endl;

    const int gridSize {9};
    const int cellsKept {40};

    const auto positionsValues = CreatePositionsValues9x9();

    const unsigned int parallelThreadsCount {std::thread::hardware_concurrency() + 2};
    //const unsigned int parallelThreadsCount {1};
    auto gridSolver = GridSolverFactory::Make(parallelThreadsCount);

    std::vector<int> durations;

    const int testExecutionCount = 5000;
    for([[gnu::unused]] int i : boost::irange(0, testExecutionCount))
    {
        auto grid = CreateGrid(gridSize, KeepRandomCells(positionsValues, cellsKept));

        try
        {
            const auto beg = std::chrono::high_resolution_clock::now();
            const auto gridStatus = gridSolver->Solve(grid);
            const auto end = std::chrono::high_resolution_clock::now();

            if (gridStatus != GridStatus::SolvedCorrectly)
            {
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - beg).count();
                durations.push_back(duration);
            }
        }
        catch(std::exception& e)
        {
            std::cout << "Couldn't solve grid because: " << e.what();
        }
    }

    std::cout << "thread number: " << parallelThreadsCount << std::endl;
    std::cout << "number of execution: " << durations.size() << std::endl;
    std::cout << "mean duration: " << boost::accumulate(durations, 0) / durations.size() << " micro seconds" << std::endl;

    return 0;
}
