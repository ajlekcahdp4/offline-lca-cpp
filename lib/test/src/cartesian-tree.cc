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

TEST (test_cartesian_tree, lca_1)
{
    std::vector seq {4, 7, 8, 1, 2, 3, 9, 5, 6};
    cartesian_tree tree {seq};
    EXPECT_EQ (tree.lowest_common_ancestor (0, 3), 1);
}

TEST (test_cartesian_tree, lca_2)
{
    std::vector seq {4, 7, 8, 1, 2, 3, 9, 5, 6};
    cartesian_tree tree {seq};
    EXPECT_EQ (tree.lowest_common_ancestor (2, 7), 1);
}

TEST (test_cartesian_tree, lca_3)
{
    std::vector seq {4, 7, 8, 1, 2, 3, 9, 5, 6};
    cartesian_tree tree {seq};
    EXPECT_EQ (tree.lowest_common_ancestor (4, 7), 2);
}