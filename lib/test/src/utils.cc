#include "utils.hpp"

#include <gtest/gtest.h>

#include <iostream>
#include <vector>

TEST (test_utils, test_lefT_neighbors)
{
    std::vector a {4, 7, 8, 1, 2, 3, 9, 5, 6};
    std::vector b = red::containers::utils::get_left_neighbors<int> (std::span {a});
    std::vector res {-1, 0, 1, -1, 3, 4, 5, 5, 7};
    EXPECT_EQ (res, b);
}