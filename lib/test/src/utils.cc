#include "utils.hpp"

#include <gtest/gtest.h>

#include <iostream>
#include <vector>

TEST (test_utils, test_left_neighbors)
{
    std::vector a {4, 7, 8, 1, 2, 3, 9, 5, 6};

    std::vector left_neighbors = red::containers::utils::get_left_neighbors (std::span {a});

    std::vector res {-1, 0, 1, -1, 3, 4, 5, 5, 7};
    EXPECT_EQ (res, left_neighbors);
}

TEST (test_utils, test_right_neighbors)
{
    std::vector a {4, 7, 8, 1, 2, 3, 9, 5, 6};

    std::vector right_neighbors = red::containers::utils::get_right_neighbors (std::span {a});

    std::vector res {3, 3, 3, -1, -1, -1, 7, -1, -1};
    EXPECT_EQ (res, right_neighbors);
}