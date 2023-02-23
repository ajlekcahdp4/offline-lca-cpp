#include "disjoint_map_forest.hpp.hpp"

#include <fstream>
#include <gtest/gtest.h>

template class red::containers::disjoint_map_forest<int, int>;
using dmf = red::containers::disjoint_map_forest<int, int>;

TEST (test_disjoint_map_forest, ctor) { dmf a; }

TEST (test_disjoint_map_forest, make_set)
{
    dmf forest;
    bool res = false;
    res      = forest.make_set (1, 1);
    EXPECT_TRUE (res);
    res = forest.make_set (2, 2);
    EXPECT_TRUE (res);
    res = forest.make_set (1, 2);
    EXPECT_FALSE (res);
}

TEST (test_disjoint_map_forest, dump)
{
    std::ofstream os {"dump-dmf.txt"};
    dmf forest;
    forest.make_set (1, 1);
    forest.make_set (2, 2);
    forest.make_set (3, 3);
    forest.union_by_rank (1, 3);
    forest.dump (os);
}
