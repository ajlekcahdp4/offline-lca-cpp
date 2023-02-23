/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <alex.rom23@mail.ru> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.   Alex Romanov
 * ----------------------------------------------------------------------------
 */

// Cartesian tree

#pragma once

#include "tree-node.hpp"
#include "utils.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <span>
#include <unordered_map>
#include <vector>

namespace red
{
namespace containers
{

template <typename T, typename compare_t = std::less<T>> class cartesian_tree final
{
    using base_node      = detail::dl_binary_tree_node_base;
    using base_node_ptr  = base_node *;
    using node           = detail::set_node<T>;
    using node_ptr       = node *;
    using const_node_ptr = const node *;

  public:
    using size_type  = std::size_t;
    using value_type = T;

    cartesian_tree () {}
#if 0
    cartesian_tree (const cartesian_tree &rhs);
#endif

    cartesian_tree (cartesian_tree &&rhs) noexcept
        : m_comp {std::move (rhs.m_comp)}, m_header {std::move (rhs.m_header)},
          m_leftmost {std::move (rhs.m_leftmost)},
          m_rightmost {std::move (rhs.m_rightmost)}, m_nodes {std::move (rhs.m_nodes)}

    {
    }

#if 0
    cartesian_tree &operator= (const cartesian_tree &rhs)
    {
        cartesian_tree tmp {rhs};
        std::swap (*this, tmp);
        return *this;
    }
#endif

    cartesian_tree &operator= (cartesian_tree &&rhs) noexcept
    {
        std::swap (m_header, rhs.m_header);
        std::swap (m_leftmost, rhs.m_leftmost);
        std::swap (m_rightmost, rhs.m_rightmost);
        std::swap (m_nodes, rhs.m_nodes);
        std::swap (m_comp, rhs.m_comp);
        return *this;
    }

    cartesian_tree (std::span<const value_type> sequence) : m_comp {}
    {
        auto left_neighbors  = utils::get_left_neighbors (sequence);
        auto right_neighbors = utils::get_right_neighbors (sequence);

        auto node_from_value = [this] (auto val, auto idx) -> node_ptr {
            if ( !m_nodes.contains (idx) )
            {
                auto to_insert = create_node (val, idx);
                return to_insert;
            }
            else
                return &m_nodes.at (idx);
        };

        for ( unsigned i = 0; i < sequence.size (); ++i )
        {
            auto r_neighbor         = right_neighbors[i];
            auto l_neighbor         = left_neighbors[i];
            auto val                = sequence[i];

            auto to_insert = node_from_value (val, i);

            if ( r_neighbor == -1 )
            {
                if ( l_neighbor == -1 )
                {
                    to_insert->m_parent = &m_header;
                    m_header.m_left     = to_insert;
                }
                else
                {
                    to_insert->m_parent = node_from_value (sequence[l_neighbor], l_neighbor);
                    to_insert->m_parent->m_right = to_insert;
                }
            }
            else
            {
                if ( l_neighbor == -1 )
                {
                    to_insert->m_parent = node_from_value (sequence[r_neighbor], r_neighbor);
                    to_insert->m_parent->m_left = to_insert;
                }
                else
                {
                    bool left = m_comp (sequence[r_neighbor], sequence[l_neighbor]) ? true : false;
                    auto parent_v = std::max (sequence[l_neighbor], sequence[r_neighbor], m_comp);
                    to_insert->m_parent =
                        node_from_value (parent_v, left ? l_neighbor : r_neighbor);
                    if ( left )
                        to_insert->m_parent->m_right = to_insert;
                    else
                        to_insert->m_parent->m_left = to_insert;
                }
            }
        }

        m_leftmost  = &m_nodes.at (0);
        m_rightmost = &m_nodes.at (sequence.size () - 1);
    }

    template <typename TT> struct set_iterator
    {
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = TT;
        using pointer           = value_type *;
        using reference         = value_type &;
        using stored_ptr_t =
            typename std::conditional_t<std::is_const_v<TT>, const_node_ptr, node_ptr>;

        reference operator* () const { return m_node->m_value; }

        pointer get () const { return &m_node->m_value; }

        pointer operator->() const { return get (); }

        set_iterator &operator++ ()
        {
            m_node = static_cast<stored_ptr_t> (m_node->successor ());
            return *this;
        }

        set_iterator operator++ (int)
        {
            auto tmp = *this;
            m_node   = static_cast<stored_ptr_t> (m_node->successor ());
            return tmp;
        }

        set_iterator &operator-- ()
        {
            m_node = (m_node ? static_cast<stored_ptr_t> (m_node->predecessor ())
                             : static_cast<stored_ptr_t> (m_tree->m_rightmost));
            return *this;
        }

        set_iterator operator-- (int)
        {
            auto tmp = *this;
            m_node   = (m_node ? static_cast<stored_ptr_t> (m_node->predecessor ())
                               : static_cast<stored_ptr_t> (m_tree->m_rightmost));
            return tmp;
        }

        bool operator== (const set_iterator &other) const { return m_node == other.m_node; }

        bool operator!= (const set_iterator &other) const { return !(*this == other); }

        stored_ptr_t m_node          = nullptr;
        const cartesian_tree *m_tree = nullptr;
    };

    using iterator               = set_iterator<value_type>;
    using const_iterator         = set_iterator<const value_type>;
    using reverse_iterator       = typename std::reverse_iterator<iterator>;
    using const_reverse_iterator = typename std::reverse_iterator<const_iterator>;

    //  accessors
    const_iterator begin () const { return const_iterator {m_leftmost, this}; }

    iterator begin () { return iterator {m_leftmost, this}; }

    const_iterator end () const { return const_iterator {nullptr, this}; }

    iterator end () { return iterator {nullptr, this}; }

    reverse_iterator rbegin () { return reverse_iterator {end ()}; }

    const_reverse_iterator rbegin () const { return const_reverse_iterator {end ()}; }

    reverse_iterator rend () { return reverse_iterator {begin ()}; }

    const_reverse_iterator rend () const { return const_reverse_iterator {begin ()}; }

    size_type size () const { return m_nodes.size (); }

    bool empty () const { return !size (); }

    void dump (std::ostream &stream) const
    {
        assert (stream);
        stream << "digraph {\nrankdir = TB\n";
        for ( auto pos = begin (); pos != end (); pos++ )
        {
            stream << "\tnode" << pos.m_node << "[label = \"" << *pos
                   << "\", shape=record, style=filled, fillcolor=palegreen];\n";

            if ( pos.m_node->m_left )
                stream << "\tnode" << pos.m_node << " -> node" << pos.m_node->m_left
                       << " [color=black, label=\"lchild\"];\n";
            else
            {
                stream << "\tnode" << pos.m_node << " -> node0_l_" << pos.m_node
                       << " [color=black, label=\"lchild\"];\n";
                stream << "\tnode0_l_" << pos.m_node
                       << " [label = \"\", shape=triangle, style=filled, fillcolor=black ];\n";
            }

            if ( pos.m_node->m_right )
                stream << "\tnode" << pos.m_node << " -> node" << pos.m_node->m_right
                       << " [color=black, label=\"rchild\"];\n";
            else
            {
                stream << "\tnode" << pos.m_node << " -> node0_r_" << pos.m_node
                       << " [color=black, label=\"rchild\"];\n";
                stream << "\tnode0_r_" << pos.m_node
                       << " [label = \"\", shape=triangle, style=filled, fillcolor=black];\n";
            }
            stream << "\tnode" << pos.m_node << " -> node" << pos.m_node->m_parent
                   << " [color=red, label=\" parent \"];\n";
        }
        stream << "\tnode_leftmost -> node" << m_leftmost << "\n";
        stream << "\tnode_rightmost -> node" << m_rightmost << "\n";
        stream << "}\n";
    }

    value_type lowest_common_ancestor (const size_type left, const size_type right)
    {
        auto left_node  = &m_nodes.at (left);
        auto right_node = &m_nodes.at (right);
        return lowest_common_ancestor_nodes (left_node, right_node);
    }

  private:
    node_ptr create_node (const value_type val, const size_type idx)
    {
        m_nodes.emplace (idx, val);
        return &m_nodes.at (idx);
    }

    std::vector<value_type> find_path_to_root (const base_node_ptr node)
    {
        std::vector<value_type> path;
        auto root = m_header.m_left;
        auto cur  = node;
        while ( cur != root )
        {
            path.push_back (node::value (cur));
            cur = cur->m_parent;
        }
        path.push_back (node::value (root));
        return path;
    }

    value_type lowest_common_ancestor_nodes (base_node_ptr left, base_node_ptr right)
    {
        auto l_path = find_path_to_root (left);
        auto r_path = find_path_to_root (right);
        auto [l_it, r_it] =
            std::mismatch (l_path.rbegin (), l_path.rend (), r_path.rbegin (), r_path.rend ());
        return *(--l_it);
    }

  private:
    compare_t m_comp {};
    node m_header;
    node_ptr m_leftmost  = nullptr;
    node_ptr m_rightmost = nullptr;
    std::unordered_map<size_type, node> m_nodes;
};

}   // namespace containers
}   // namespace red