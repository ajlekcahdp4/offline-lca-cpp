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

#include <iostream>
#include <memory>
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

  protected:
    base_node_ptr m_header    = nullptr;
    base_node_ptr m_leftmost  = nullptr;
    base_node_ptr m_rightmost = nullptr;
    std::unordered_set<std::unique_ptr<base_node>, unique_node_ptr_hash, unique_node_ptr_equal>
        m_nodes;
    std::size_t m_size {};
};

template <typename T> class cartesian_tree
{
    using base_node      = dl_binary_tree_node_base;
    using base_node_ptr  = base_node *;
    using node           = set_node<T>;
    using node_ptr       = node *;
    using const_node_ptr = const node *;

  public:
    using size_type  = typename std::size_t;
    using value_type = T;

    virtual ~cartesian_tree () {}

    cartesian_tree () {}
#if 0
    cartesian_tree (const cartesian_tree &rhs);
#endif

    cartesian_tree (cartesian_tree &&rhs) noexcept
        : m_header {std::move (rhs.m_header)}, m_sequence {std::move (rhs.m_sequence)}
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
        std::swap (m_sequence, rhs.m_sequence);
        return *this;
    }

  protected:
    header m_header {};
    std::vector<value_type> m_sequence;
};

}   // namespace containers
}   // namespace red