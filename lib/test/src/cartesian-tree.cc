#include "cartesian-tree.hpp"

#include <gtest/gtest.h>

template class red::containers::cartesian_tree<int>;
using cartesian_tree = typename red::containers::cartesian_tree<int>;

TEST (test_cartesian_tree, test_ctor)
{
    cartesian_tree a;
    cartesian_tree c {std::move (a)};
    a = std::move (c);
}