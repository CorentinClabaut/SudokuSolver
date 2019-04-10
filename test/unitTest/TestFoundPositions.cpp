#include "FoundPositions.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <thread>
#include <functional>

#include <boost/range/irange.hpp>
#include <boost/range/algorithm.hpp>

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

class TestFoundPositions : public ::testing::Test
{
public:
    TestFoundPositions()
    {}

    std::vector<Position> QueueToVector(std::queue<Position>& queue)
    {
        std::vector<Position> pushedPositions;

        while (!queue.empty())
        {
            pushedPositions.push_back(queue.front());
            queue.pop();
        }

        return pushedPositions;
    }

    std::function<void(void)> EnqueuePositionXtimes(Position const& position, int times)
    {
        return [=]{
                for([[gnu::unused]] int i : boost::irange(0, times))
                {
                    m_FoundPositions.Enqueue(position);
                }
            };
    }

    FoundPositions m_FoundPositions;
};

TEST_F(TestFoundPositions, Enqueue)
{
    m_FoundPositions.Enqueue(Position{0, 0});

    EXPECT_THAT(m_FoundPositions.m_Queue.size(), Eq(1));
    EXPECT_THAT(m_FoundPositions.m_Queue.front(), Eq(Position{0, 0}));
}

TEST_F(TestFoundPositions, SeveralThreadsEnqueue)
{
    std::vector<Position> positions = {
        Position{0, 0},
        Position{1, 1},
        Position{2, 2},
        Position{3, 3}
    };

    const int testExecutionCount = 20;
    for([[gnu::unused]] int i : boost::irange(0, testExecutionCount))
    {
        const int positionsToAddPerThreadCount = 20;

        std::vector<std::thread> threads;

        boost::range::transform(positions, std::back_inserter(threads), [=](auto position){ return std::thread(EnqueuePositionXtimes(position, positionsToAddPerThreadCount)); });

        boost::range::for_each(threads, [](auto& thread){ thread.join(); });

        auto pushedPositions = QueueToVector(m_FoundPositions.m_Queue);

        boost::range::for_each(positions, [&](auto const& position){
            EXPECT_THAT(boost::range::count(pushedPositions, position), Eq(positionsToAddPerThreadCount));
        });
    }
}

} /* namespace test */
} /* namespace sudoku */
