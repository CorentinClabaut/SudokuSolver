#include "FoundCellsEnqueuer.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <thread>
#include <functional>

#include <boost/range/irange.hpp>
#include <boost/range/algorithm.hpp>

#include "FoundCells.hpp"

using testing::_;
using testing::Eq;
using testing::Ref;
using testing::Return;
using testing::ReturnRef;
using testing::StrictMock;

namespace sudoku
{
namespace test
{

class TestFoundCellsEnqueuer : public ::testing::Test
{
public:
    TestFoundCellsEnqueuer() :
        m_FoundCellsEnqueuer(m_FoundCells)
    {}

    std::vector<SharedCell> QueueToVector(std::queue<SharedCell>& queue)
    {
        std::vector<SharedCell> pushedCells;

        while (!queue.empty())
        {
            pushedCells.push_back(queue.front());
            queue.pop();
        }

        return pushedCells;
    }

    std::function<void(void)> EnqueueCellXtimes(SharedCell cell, int times)
    {
        return [=]{
                for([[gnu::unused]] int i : boost::irange(0, times))
                {
                    m_FoundCellsEnqueuer.Enqueue(cell);
                }
            };
    }

    std::shared_ptr<FoundCells> m_FoundCells = std::make_shared<FoundCells>();

    FoundCellsEnqueuerImpl m_FoundCellsEnqueuer;
};

TEST_F(TestFoundCellsEnqueuer, Enqueue)
{
    SharedCell cell = std::make_shared<Cell>(Position{0, 0}, 4);

    m_FoundCellsEnqueuer.Enqueue(cell);

    EXPECT_THAT(m_FoundCells->m_Queue.size(), Eq(1));
    EXPECT_THAT(m_FoundCells->m_Queue.front(), Eq(cell));
}

TEST_F(TestFoundCellsEnqueuer, SeveralThreadsEnqueue)
{
    std::vector<SharedCell> cells = {
        std::make_shared<Cell>(Position{0, 0}, 1),
        std::make_shared<Cell>(Position{1, 1}, 2),
        std::make_shared<Cell>(Position{2, 2}, 3),
        std::make_shared<Cell>(Position{3, 3}, 4)
    };

    const int testExecutionCount = 20;
    for([[gnu::unused]] int i : boost::irange(0, testExecutionCount))
    {
        const int cellsToAddPerThreadCount = 20;

        std::vector<std::thread> threads;

        boost::range::transform(cells, std::back_inserter(threads), [=](auto cell){ return std::thread(EnqueueCellXtimes(cell, cellsToAddPerThreadCount)); });

        boost::range::for_each(threads, [](auto& thread){ thread.join(); });

        auto pushedCells = QueueToVector(m_FoundCells->m_Queue);

        boost::range::for_each(cells, [&](auto cell){ EXPECT_THAT(boost::range::count(pushedCells, cell), Eq(cellsToAddPerThreadCount)); });
    }
}

} /* namespace test */
} /* namespace sudoku */
