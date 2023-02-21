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

#include <cassert>
#include <iostream>
#include <memory>
#include <span>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace red
{
namespace containers
{

// helper type to manage deafault initialization of node count, header and boundary nodes.
// stores all tree's nodes as unique pointers.
class header
{
  protected:
    using base_node     = dl_binary_tree_node_base;
    using base_node_ptr = dl_binary_tree_node_base *;

    struct unique_node_ptr_hash
    {
        std::size_t operator() (const std::unique_ptr<base_node> &unique_ptr) const
        {
            return reinterpret_cast<std::size_t> (unique_ptr.get ());
        }
    };

    struct unique_node_ptr_equal
    {
        bool operator() (const std::unique_ptr<base_node> &lhs,
                         const std::unique_ptr<base_node> &rhs) const
        {
            return lhs.get () == rhs.get ();
        }
    };

  public:
    virtual ~header () {}

    header ()
    {
        auto node = std::make_unique<base_node> ();
        m_header  = node.get ();
        m_nodes.emplace (std::move (node));
    }

    header (const header &) = delete;

    header (header &&rhs)
        : m_header {std::move (rhs.m_header)}, m_leftmost {std::move (rhs.m_leftmost)},
          m_rightmost {std::move (rhs.m_rightmost)}, m_nodes {std::move (rhs.m_nodes)},
          m_size {std::move (rhs.m_size)}
    {
    }

    header &operator= (const header &) = delete;
    header &operator= (header &&rhs)
    {
        std::swap (m_leftmost, rhs.m_leftmost);
        std::swap (m_rightmost, rhs.m_rightmost);
        std::swap (m_header, rhs.m_header);
        std::swap (m_nodes, rhs.m_nodes);
        std::swap (m_size, rhs.m_size);
        return *this;
    }

    // Inserts to_insert into m_nodes. Performs check that to_insert has not already been inserted
    // (Otherwise releases to_insert to avoid double free and throws an exeption).
    void insert_node (std::unique_ptr<base_node> &&to_insert)
    {
        if ( m_nodes.count (to_insert) )
        {
            to_insert.release ();
            throw std::runtime_error ("double insert of the node");
        }
        m_nodes.emplace (std::move (to_insert));
    }

    void m_reset ()
    {
        m_leftmost  = nullptr;
        m_rightmost = nullptr;
        m_header    = nullptr;
        m_size      = 0;
        m_nodes.clear ();
        auto node = std::make_unique<base_node> ();
        m_header  = node.get ();
        m_nodes.emplace (std::move (node));
    }

    base_node_ptr m_header    = nullptr;
    base_node_ptr m_leftmost  = nullptr;
    base_node_ptr m_rightmost = nullptr;
    std::unordered_set<std::unique_ptr<base_node>, unique_node_ptr_hash, unique_node_ptr_equal>
        m_nodes;
    std::size_t m_size {};
};

template <typename T> class cartesian_tree final
{
    using base_node      = dl_binary_tree_node_base;
    using base_node_ptr  = base_node *;
    using node           = set_node<T>;
    using node_ptr       = node *;
    using const_node_ptr = const node *;

  public:
    using size_type  = typename std::size_t;
    using value_type = T;

    cartesian_tree () {}
#if 0
    cartesian_tree (const cartesian_tree &rhs);
#endif

    cartesian_tree (cartesian_tree &&rhs) noexcept
        : m_header_struct {std::move (rhs.m_header_struct)}
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
        std::swap (m_header_struct, rhs.m_header_struct);
        return *this;
    }

    cartesian_tree (std::span<const value_type> sequence)
    {
        auto left_neighbors  = utils::get_left_neighbors (sequence);
        auto right_neighbors = utils::get_right_neighbors (sequence);
        std::unordered_map<value_type, base_node_ptr> allocated;

        for ( unsigned i = 0; i < sequence.size (); ++i )
        {
            auto r_neighbor         = right_neighbors[i];
            auto l_neighbor         = left_neighbors[i];
            auto val                = sequence[i];
            base_node_ptr to_insert = nullptr;

            if ( allocated.count (val) == 0 )
            {
                to_insert      = create_node (val);
                allocated[val] = to_insert;
            }
            else
                to_insert = allocated.at (val);

            if ( r_neighbor == -1 )
            {
                if ( l_neighbor == -1 )
                {
                    to_insert->m_parent              = m_header_struct.m_header;
                    m_header_struct.m_header->m_left = to_insert;
                }
                else
                {
                    auto parent_v = sequence[l_neighbor];
                    if ( allocated.count (parent_v) == 0 )
                    {
                        to_insert->m_parent = create_node (parent_v);
                        allocated[parent_v] = to_insert->m_parent;
                    }
                    else
                        to_insert->m_parent = allocated.at (parent_v);
                    to_insert->m_parent->m_right = to_insert;
                }
            }
            else
            {
                if ( l_neighbor == -1 )
                {
                    auto parent_v = sequence[r_neighbor];
                    if ( allocated.count (parent_v) == 0 )
                    {
                        to_insert->m_parent = create_node (parent_v);
                        allocated[parent_v] = to_insert->m_parent;
                    }
                    else
                        to_insert->m_parent = allocated.at (parent_v);
                    to_insert->m_parent->m_left = to_insert;
                }
                else
                {
                    bool left     = sequence[l_neighbor] > sequence[r_neighbor] ? true : false;
                    auto parent_v = std::max (sequence[l_neighbor], sequence[r_neighbor]);
                    if ( allocated.count (parent_v) == 0 )
                    {
                        to_insert->m_parent = create_node (parent_v);
                        allocated[parent_v] = to_insert->m_parent;
                    }
                    else
                        to_insert->m_parent = allocated.at (parent_v);
                    if ( left )
                        to_insert->m_parent->m_right = to_insert;
                    else
                        to_insert->m_parent->m_left = to_insert;
                }
            }
        }

        m_header_struct.m_leftmost  = allocated.at (sequence.front ());
        m_header_struct.m_rightmost = allocated.at (sequence.back ());
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
                             : static_cast<stored_ptr_t> (m_tree->m_header_struct.m_rightmost));
            return *this;
        }

        set_iterator operator-- (int)
        {
            auto tmp = *this;
            m_node   = (m_node ? static_cast<stored_ptr_t> (m_node->predecessor ())
                               : static_cast<stored_ptr_t> (m_tree->m_header_struct.m_rightmost));
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
    const_iterator begin () const
    {
        return const_iterator {static_cast<node_ptr> (m_header_struct.m_leftmost), this};
    }

    iterator begin ()
    {
        return iterator {static_cast<node_ptr> (m_header_struct.m_leftmost), this};
    }

    const_iterator end () const { return const_iterator {nullptr, this}; }

    iterator end () { return iterator {nullptr, this}; }

    reverse_iterator rbegin () { return reverse_iterator {end ()}; }

    const_reverse_iterator rbegin () const { return const_reverse_iterator {end ()}; }

    reverse_iterator rend () { return reverse_iterator {begin ()}; }

    const_reverse_iterator rend () const { return const_reverse_iterator {begin ()}; }

    size_type size () const { return m_header_struct.m_size; }

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
        stream << "\tnode_leftmost -> node" << m_header_struct.m_leftmost << "\n";
        stream << "\tnode_rightmost -> node" << m_header_struct.m_rightmost << "\n";
        stream << "}\n";
    }

  private:
    void insert_node_to_nodes (std::unique_ptr<base_node> &&node_uptr)
    {
        m_header_struct.insert_node (std::move (node_uptr));
    }

    base_node_ptr create_node (const value_type val)
    {
        auto to_insert_u = std::make_unique<node> (val);
        auto to_insert   = to_insert_u.get ();
        m_header_struct.insert_node (std::move (to_insert_u));
        return to_insert;
    }

  private:
    header m_header_struct {};
};

}   // namespace containers
}   // namespace red