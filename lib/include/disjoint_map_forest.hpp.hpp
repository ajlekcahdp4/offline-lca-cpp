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
#include <unordered_map>
#include <vector>

namespace red
{
namespace containers
{

namespace detail
{

template <typename T> struct disjoint_map_forest_node final
{
    using size_type  = unsigned;
    using value_type = T;

    size_type m_parent_index {};
    size_type m_rank {};
    value_type m_value {};

    disjoint_map_forest_node (size_type p_parent_index, value_type val)
        : m_parent_index {p_parent_index}, m_rank {0}, m_value {val}
    {
    }
};

}   // namespace detail

template <typename key_t, typename value_t> class disjoint_map_forest final
{
    using node_type = detail::disjoint_map_forest_node<value_t>;

  public:
    using key_type   = key_t;
    using value_type = typename node_type::value_type;
    using size_type  = typename node_type::size_type;

    disjoint_map_forest () {}

    bool make_set (const key_type &key, const value_type &val)
    {
        size_type idx = m_nodes.size ();
        bool inserted = m_indeces.emplace (key, index).second;
        if ( inserted )
        {
            m_nodes.emplace_back (idx, val);
            return true;
        }
        return false;
    }

    struct proxy_set
    {
        using pointer   = value_type *;
        using reference = value_type &;

        proxy_set (size_type node_idx, disjoint_map_forest *map)
            : m_index {node_idx}, m_forest {map}
        {
        }

        reference operator* () { return m_forest->at (m_index).m_value; }
        pointer operator->() { return &(m_forest->at (m_index).m_value); }

      private:
        size_type m_index;
        disjoint_map_forest *m_forest;
    };

    size_type find_set_index (const key_type &key)
    {
        std::vector<size_type> stack;
        auto node_idx    = m_indeces.at (key);
        auto &parent_idx = m_nodes.at (node_idx).m_parent_index;
        while ( parent != node_idx )
        {
            stack.push_back (node_idx);
            node_idx   = parent_idx;
            parent_idx = parent_index (parent_idx);
        }
        while ( !stack.empty () )
        {
            parent_idx (stack.back ()) = node_idx;
            stack.pop_back ();
        }
        return parent_idx;
    }

    proxy_set find_set (const key_type &key) { return proxy_set {find_set_index (key), this}; }

    void union_by_rank (const key_type &left_key, const key_rype &right_key)
    {
        if ( left_key != right_key )
        {
            auto left_idx   = find_set_index (left_key);
            auto right_idx  = find_set_index (left_key);
            auto left_node  = m_nodes.at (left_idx);
            auto right_node = m_nodes.at (right_idx);

            if ( left_node.m_rank > right_node.m_rank )
                right_node.m_parent_index = left_idx;
            else
            {
                left_node.m_parent_index = right_idx;
                if ( left_node.m_rank == right_node.m_rank )
                    ++right_node.m_rank;
            }
        }
    }

    void dump (std::ostream &os) const
    {
        os << "digraph {\n";
        for ( const auto &v : m_indices )
        {
            os << "\tnode_" << v.second << " [label = \"" << v.first << ":"
               << m_nodes.at (v.second).m_val << "\"];\n";
            os << "\tnode_" << v.second << " -> node_" << parent_index (v.second) << ";\n";
        }
        os << "}\n";
    }

  private:
    node_type &parent_index (const size_type idx) { return m_nodes.at (idx)->m_parent_index; }

  private:
    std::vector<node_type> m_nodes;
    std::unordered_map<key_type, size_type> m_indeces;
};

template <typename key_t, typename value_t>
std::ostream operator<< (std::ostream &os, const disjoint_map_forest<key_t, value_t> &forest)
{
    forest.dump (os);
    return os;
}

}   // namespace containers
}   // namespace red