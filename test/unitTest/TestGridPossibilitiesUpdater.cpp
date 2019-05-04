#include "GridPossibilitiesUpdater.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "FoundPositions.hpp"
#include "Grid.hpp"

#include "mock/MockRelatedPossibilitiesRemover.hpp"

using testing::_;
using testing::Eq;
using testing::Ref;
using testing::Return;
using testing::Invoke;
using testing::StrictMock;

namespace sudoku
{
namespace test
{

class TestGridPossibilitiesUpdater : public ::testing::Test
{
public:
    TestGridPossibilitiesUpdater()
    {}

    std::unique_ptr<GridPossibilitiesUpdater> MakeGridPossibilitiesUpdater()
    {
        return std::make_unique<GridPossibilitiesUpdaterImpl>(
                    std::move(m_RelatedPossibilitiesRemover));
    }

    const int m_GridSize {4};

    std::shared_ptr<FoundPositions> m_FoundPositions = std::make_shared<FoundPositions>();
    Grid m_Grid {m_GridSize};

    std::unique_ptr<MockRelatedPossibilitiesRemover> m_RelatedPossibilitiesRemover = std::make_unique<StrictMock<MockRelatedPossibilitiesRemover>>();
};

TEST_F(TestGridPossibilitiesUpdater, UpdateGridWithEmptyFoundPositions)
{
    EXPECT_CALL(*m_RelatedPossibilitiesRemover, UpdateRelatedPossibilities(_, Ref(m_Grid), Ref(*m_FoundPositions))).Times(0);

    MakeGridPossibilitiesUpdater()->UpdateGrid(*m_FoundPositions, m_Grid);

    EXPECT_TRUE(m_FoundPositions->empty());
}

TEST_F(TestGridPossibilitiesUpdater, UpdateGrid)
{
    m_FoundPositions->push(Position {0, 0});
    m_FoundPositions->push(Position {0, 1});
    m_FoundPositions->push(Position {0, 2});

    EXPECT_CALL(*m_RelatedPossibilitiesRemover, UpdateRelatedPossibilities(Position {0, 0}, Ref(m_Grid), Ref(*m_FoundPositions))).Times(1);
    EXPECT_CALL(*m_RelatedPossibilitiesRemover, UpdateRelatedPossibilities(Position {0, 1}, Ref(m_Grid), Ref(*m_FoundPositions))).Times(1);
    EXPECT_CALL(*m_RelatedPossibilitiesRemover, UpdateRelatedPossibilities(Position {0, 2}, Ref(m_Grid), Ref(*m_FoundPositions))).Times(1);

    MakeGridPossibilitiesUpdater()->UpdateGrid(*m_FoundPositions, m_Grid);

    EXPECT_TRUE(m_FoundPositions->empty());
}

} /* namespace test */
} /* namespace sudoku */
