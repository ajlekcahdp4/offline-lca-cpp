/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <alex.rom23@mail.ru> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.   Alex Romanov
 * ----------------------------------------------------------------------------
 */

// defenitions for some functions declared in "tree_node.hpp"

#include "tree-node.hpp"

namespace red
{
namespace containers
{

dl_binary_tree_node_base *dl_binary_tree_node_base::rotate_left_base ()
{
    auto node       = this;
    auto parent     = this->m_parent;
    auto rchild_ptr = node->m_right;
    node->m_right   = rchild_ptr->m_left;
    if ( node->m_right )
        node->m_right->m_parent = node;

    rchild_ptr->m_parent = parent;
    if ( parent )
    {
        rchild_ptr->m_left = node;
        if ( node->is_left_child () )
            parent->m_left = rchild_ptr;
        else
            parent->m_right = rchild_ptr;
    }
    node->m_parent = rchild_ptr;
    return rchild_ptr;
}

dl_binary_tree_node_base *dl_binary_tree_node_base::rotate_right_base ()
{
    auto node       = this;
    auto parent     = this->m_parent;
    auto lchild_ptr = node->m_left;
    node->m_left    = lchild_ptr->m_right;
    if ( node->m_left )
        node->m_left->m_parent = node;
    lchild_ptr->m_parent = parent;
    if ( parent )
    {
        lchild_ptr->m_right = node;
        if ( node->is_left_child () )
            parent->m_left = lchild_ptr;
        else
            parent->m_right = lchild_ptr;
    }
    node->m_parent = lchild_ptr;
    return lchild_ptr;
}

}   // namespace containers
}   // namespace red