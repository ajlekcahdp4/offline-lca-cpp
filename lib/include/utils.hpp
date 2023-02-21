/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <alex.rom23@mail.ru> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.   Alex Romanov
 * ----------------------------------------------------------------------------
 */

#pragma once

#include <iostream>
#include <span>
#include <vector>

namespace red::containers::utils
{

template <typename T> std::vector<int> get_left_neighbors (std::span<const T> sequence)
{
    std::vector<unsigned> stack {};
    std::vector<int> neighbors {};
    for ( unsigned i = 0; i < sequence.size (); ++i )
    {
        while ( (!stack.empty ()) && (sequence[i] <= sequence[stack.back ()]) )
            stack.pop_back ();

        if ( stack.empty () )
            neighbors.push_back (-1);
        else
            neighbors.push_back (stack.back ());
        stack.push_back (i);
    }
    return neighbors;
}

}   // namespace red::containers::utils