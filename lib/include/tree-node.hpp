/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <alex.rom23@mail.ru> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.   Alex Romanov
 * ----------------------------------------------------------------------------
 */

// node types for binary tree

#pragma once

namespace red
{
namespace containers
{

struct dl_binary_tree_node_base
{
    dl_binary_tree_node_base *m_parent = nullptr;
    dl_binary_tree_node_base *m_left   = nullptr;
    dl_binary_tree_node_base *m_right  = nullptr;

    virtual ~dl_binary_tree_node_base () {};

    virtual dl_binary_tree_node_base *rotate_left () { return rotate_left_base (); }

    virtual dl_binary_tree_node_base *rotate_right () { return rotate_right_base (); }

    bool is_left_child () const { return (m_parent ? this == m_parent->m_left : false); }

    bool is_linear () const { return m_parent && (is_left_child () == m_parent->is_left_child ()); }

    dl_binary_tree_node_base *rotate_left_base ();

    dl_binary_tree_node_base *rotate_right_base ();

    dl_binary_tree_node_base *successor () { return successor_base (); }

    const dl_binary_tree_node_base *successor () const { return successor_base (); }

    dl_binary_tree_node_base *successor_base () const
    {
        auto curr = this;
        if ( curr->m_right )
            return curr->m_right->minimum ();
        auto *prev = curr->m_parent;
        while ( prev->m_parent && !curr->is_left_child () )
        {
            curr = prev;
            prev = prev->m_parent;
        }
        if ( prev->m_parent )
            return prev;
        return nullptr;
    }

    template <typename F> dl_binary_tree_node_base *successor_step (F step)
    {
        auto curr = this;
        if ( curr->m_right )
            return curr->m_right->minimum (step);
        auto *prev = curr->m_parent;
        while ( prev->m_parent && !curr->is_left_child () )
        {
            curr = prev;
            prev = prev->m_parent;
        }
        if ( prev->m_parent )
            return prev;
        return nullptr;
    }

    dl_binary_tree_node_base *predecessor () { return predecessor_base (); }

    const dl_binary_tree_node_base *predecessor () const { return predecessor_base (); }

    dl_binary_tree_node_base *predecessor_base () const
    {
        auto curr = this;
        if ( curr->m_left )
            return curr->m_left->maximum ();
        auto prev = curr->m_parent;
        while ( prev->m_parent && curr->is_left_child () )
        {
            curr = prev;
            prev = prev->m_parent;
        }
        if ( prev->m_parent )
            return prev;
        return nullptr;
    }

    template <typename F> dl_binary_tree_node_base *predecessor_step (F step)
    {
        auto curr = this;
        if ( curr->m_left )
            return curr->m_left->maximum (step);
        auto prev = curr->m_parent;
        while ( prev->m_parent && curr->is_left_child () )
        {
            curr = prev;
            prev = prev->m_parent;
        }
        if ( prev->m_parent )
            return prev;
        return nullptr;
    }

    const dl_binary_tree_node_base *minimum () const
    {
        auto node = this;
        while ( node->m_left )
            node = node->m_left;
        return node;
    };

    dl_binary_tree_node_base *minimum ()
    {
        auto node = this;
        while ( node->m_left )
            node = node->m_left;
        return node;
    };

    template <typename F> dl_binary_tree_node_base *minimum (F step)
    {
        auto node = this;
        while ( node->m_left )
        {
            step (node);
            node = node->m_left;
        }
        return node;
    };

    const dl_binary_tree_node_base *maximum () const
    {
        auto node = this;
        while ( node->m_right )
            node = node->m_right;
        return node;
    };

    dl_binary_tree_node_base *maximum ()
    {
        auto node = this;
        while ( node->m_right )
            node = node->m_right;
        return node;
    };

    template <typename F> dl_binary_tree_node_base *maximum (F step)
    {
        auto node = this;
        while ( node->m_right )
        {
            step (node);
            node = node->m_right;
        }
        return node;
    };

    dl_binary_tree_node_base *rotate_to_parent ()
    {
        if ( is_left_child () )
            return m_parent->rotate_right ();
        else
            return m_parent->rotate_left ();
    }
};

template <typename T> struct set_node : public dl_binary_tree_node_base
{
    using value_type          = T;
    using base_node           = dl_binary_tree_node_base;
    using base_node_ptr       = base_node *;
    using const_base_node_ptr = const base_node *;

    value_type m_value {};

    static const value_type value (const_base_node_ptr base_ptr)
    {
        return static_cast<const set_node<value_type> *> (base_ptr)->m_value;
    }

    static value_type &value (base_node_ptr base_ptr)
    {
        return static_cast<set_node<value_type> *> (base_ptr)->m_value;
    }

    set_node (T val) : m_value {val} {}
};

}   // namespace containers
}   // namespace red