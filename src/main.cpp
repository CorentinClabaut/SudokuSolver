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

//cellsKept 20
//6 threads
//time over 1000 exec : 28'857 micro seconds

//after few algo improvement and stop checking status in SolverWithoutHypothesis.
//time over 1000 exec : 15'857 micro seconds

//optimisations steps:
//run profiler on program and modify algorithm in order to delete bottlenek


int main ()
{
    std::cout << "Multithreaded Sudoku Solver" << std::endl;

    const int gridSize {9};
    const int cellsKept {20};

    const auto positionsValues = CreatePositionsValues9x9();

    const unsigned int parallelThreadsCount {std::thread::hardware_concurrency() + 2};
    auto gridSolver = GridSolverFactory::Make(parallelThreadsCount);

    std::vector<int> durations;

    const int testExecutionCount = 1000;
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
            std::cout << "Couldn't solve grid " << grid << std::endl << " because: " << e.what() << std::endl;
        }
    }

    std::cout << "thread number: " << parallelThreadsCount << std::endl;
    std::cout << "number of execution: " << durations.size() << std::endl;
    std::cout << "mean duration: " << boost::accumulate(durations, 0) / durations.size() << " micro seconds" << std::endl;

    return 0;
}
