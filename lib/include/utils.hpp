/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <alex.rom23@mail.ru> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.   Alex Romanov
 * ----------------------------------------------------------------------------
 */

#pragma once

#include <algorithm>
#include <iostream>
#include <span>
#include <vector>

namespace red::containers::utils
{

template <typename T, typename F> std::vector<int> get_neighbors (std::span<T> sequence, F loop)
{
    std::vector<unsigned> stack {};
    std::vector<int> neighbors {};

    auto iteration = [&stack, &neighbors, &sequence] (auto i) {
        while ( (!stack.empty ()) && (sequence[i] <= sequence[stack.back ()]) )
            stack.pop_back ();

        if ( stack.empty () )
            neighbors.push_back (-1);
        else
            neighbors.push_back (stack.back ());
        stack.push_back (i);
    };

    loop (iteration);
    return neighbors;
}

template <typename T> std::vector<int> get_left_neighbors (std::span<T> sequence)
{
    auto size = sequence.size ();

    auto straight_loop = [size] (auto iteration) {
        for ( unsigned i = 0; i < size; ++i )
            iteration (i);
    };

    return get_neighbors (sequence, straight_loop);
}

template <typename T> std::vector<int> get_right_neighbors (std::span<T> sequence)
{
    auto size = sequence.size ();

    auto reverse_loop = [size] (auto iteration) {
        for ( int i = size - 1; i >= 0; --i )
            iteration (i);
    };

    auto right_neighbors = get_neighbors (sequence, reverse_loop);
    std::reverse (right_neighbors.begin (), right_neighbors.end ());
    return right_neighbors;
}

}   // namespace red::containers::utils