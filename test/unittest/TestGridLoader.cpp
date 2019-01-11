#include "GridLoader.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using testing::Eq;

namespace sudoku
{

class TestGridLoader : public ::testing::Test
{
public:
    TestGridLoader()
    {}

    GridLoader m_GridLoader;
};

} // namespace sudoku
