#include "cartesian-tree.hpp"

#include <fstream>
#include <gtest/gtest.h>

template class red::containers::cartesian_tree<int>;
using cartesian_tree = typename red::containers::cartesian_tree<int>;

TEST (test_cartesian_tree, move_ctor)
{
    cartesian_tree a;
    cartesian_tree b {std::move (a)};
}

TEST (test_cartesian_tree, move_assignment)
{
    cartesian_tree a;
    cartesian_tree b;
    b = std::move (a);
}

TEST (test_cartesian_tree, sequence_ctor)
{
    std::ofstream os {"dump.txt"};
    std::vector seq {4, 7, 8, 1, 2, 3, 9, 5, 6};
    cartesian_tree tree {seq};
    tree.dump (os);
}
