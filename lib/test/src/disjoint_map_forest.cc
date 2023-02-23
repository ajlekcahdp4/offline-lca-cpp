#include "disjoint_map_forest.hpp.hpp"

#include <fstream>
#include <gtest/gtest.h>

template class red::containers::disjoint_map_forest<int>;
using dmf = red::containers::disjoint_map_forest<int>;

TEST (test_disjoint_map_forest, ctor) { dmf a; }

TEST (test_disjoint_map_forest, make_set)
{
    dmf forest;
    forest.append_set (1);
    forest.append_set (2);
    forest.append_set (2);
}

TEST (test_disjoint_map_forest, dump)
{
    std::ofstream os {"dump-dmf.txt"};
    dmf forest;
    forest.append_set (1);
    forest.append_set (2);
    forest.append_set (3);
    forest.union_by_rank (0, 2);
    forest.dump (os);
}
