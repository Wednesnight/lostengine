/**
 * $Id: ntree.h 319 2009-06-08 22:17:47Z unwesen $
 *
 * This file is part of the Fhtagn! C++ Library.
 * Copyright (C) 2009 Jens Finkhaeuser <unwesen@users.sourceforge.net>.
 *
 * Author: Jens Finkhaeuser <unwesen@users.sourceforge.net>
 *
 * This program is licensed as free software for personal, educational or
 * other non-commerical uses: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Alternatively, licenses for commercial purposes are available as well.
 * Please send your enquiries to the copyright holder's address above.
 **/
#ifndef FHTAGN_CONTAINERS_NTREE_H
#define FHTAGN_CONTAINERS_NTREE_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <fhtagn/fhtagn.h>

#include <boost/cstdint.hpp>
#include <boost/concept_check.hpp>

namespace fhtagn {
namespace containers {

/**
 * The n_tree class implements a container that is organized like a tree, with
 * each non-leaf node containing up to N (no more) child nodes. It can be used
 * as a quadtree or octree, but should not be considered a full implementation
 * of either, as it makes no assumption of e.g. the spatial relationship between
 * two nodes (quadtrees and octrees usually do).
 *
 * On the other hand, it extends the underlying principle of quadtrees and
 * octrees to trees with other child node counts than four or eight.
 *
 * One other major difference between quadtrees and octrees on the one hand,
 * and n_tree on the other, is that n_tree can allow less than N child nodes
 * in a non-leaf node. This is simply a minor space-saving optimization, but it
 * means that you cannot rely on child nodes existing:
 *
 *    for (iter i = my_n_tree.begin() ; i != my_n_tree.end() ; ++i) {
 *      i->doSomething(); // can crash...
 *
 *      // ... but the following is safe.
 *      if (i) {
 *        i->doSomething();
 *      }
 *    }
 *
 * That effectively makes n_tree a sparse tree by default - but you can change
 * this behaviour if you need.
 *
 * As seen in the above example, n_tree tries to behave a little like an STL
 * container. The similarity ends when it comes to inserting child nodes: you
 * cannot just add data to an n_tree.
 *
 * What you can do, if the n_tree is a leaf-node, is to turn it into a non-leaf
 * node via the split() operation. From that point on, iterating over an n_tree's
 * children returns results (though the iterators might not dereference to
 * anything). You can revert the operation with the join() operation, which
 * discards all child nodes of the node you perform the operation on.
 *
 * Lastly, n_tree's children are all n_tree objects themselves. Each n_tree
 * provides a pointer-like interface to it's value, in addition to the
 * container-like interface for accessing it's children.
 *
 * n_tree's exact behaviour can be modified with extra template parameters. In
 * that way, it's possible to do any of the following:
 *
 * 1. Create an n_tree where non-leaf nodes can have values -- or not.
 * 2. Create child nodes if a leaf node is split -- the default would be to
 *    not create child nodes until they're used.
 * 3. Customize what happens when a non-leaf node is joined.
 * 4. Make it possible for n_tree to contain any number of child nodes, with
 *    no particular upper limit.
 * 5. Lastly, n_tree lets you customize how it references nodes (see FIXME for
 *    details).
 **/
namespace detail {

/**
 * Forward-delcaration of child_container, the container holding child nodes of
 * a node. For non-zero CHILD_NODES values, this is little more than a simple
 * wrapper around an array. When CHILD_NODES is zero, though, it becomes a
 * std::vector. On the one hand that allows each node in the tree to have an
 * arbitrary amount of children, which can be desirable - but on the other hand,
 * there's a slightly larger space and performance overhead than non-zero child
 * nodes incur.
 **/
template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT
>
struct child_container;


/**
 * Each n_tree instance derives from a ContainedStore, the purpose of which is
 * to define a mapping from a key type to a value type pointer.
 *
 * In the case of the default_contained_store, the key type *is* the value type
 * pointer, which makes n_tree behave like many STL containers in terms of how
 * it references node values.
 *
 * Other ContainedStore implementations could e.g.
 * - Reduce the number of value objects by performing reference counting.
 * - Save space in trees with few values and many nodes, by choosing a small
 *   key type.
 * - Optimize performance by managing a large array of values, and treating
 *   keys as indices into this array.
 * - etc.
 **/
template <
  typename containedT,
  typename allocatorT
>
struct default_contained_store;


} // namespace detail



namespace concepts {

/**
 * ContainedStore concept
 **/
template <
  typename storeT
>
struct ContainedStoreConcept
{
  void constraints()
  {
    key = storeT::invalid_key();
    kv = store.create();
    kv = store.copy(key);
    store.discard(key);

    const_constraints();
  }

  void const_constraints() const
  {
    ptr = store.retrieve(key);
  }

  typename storeT::contained_ptr &  ptr;
  typename storeT::key_type &       key;
  typename storeT::key_value_t &    kv;

  storeT &                          store;
};

} // namespace concepts


template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT = std::allocator<containedT>,
  typename contained_storeT = detail::default_contained_store<
    containedT,
    allocatorT
  >
>
class n_tree
  // n_tree is an allocator of n_tree
  : public allocatorT::template rebind<
      n_tree<
        CHILD_NODES,
        containedT,
        allocatorT,
        contained_storeT
      >
    >::other
  // n_tree is also an allocator of detail::child_container parametrized
  // to hold up to CHILD_NODES of n_tree instances
  , public allocatorT::template rebind<
      detail::child_container<
        CHILD_NODES,
        n_tree<
          CHILD_NODES,
          containedT,
          allocatorT,
          contained_storeT
        >,
        typename allocatorT::template rebind<
          n_tree<
            CHILD_NODES,
            containedT,
            allocatorT,
            contained_storeT
          >
        >::other
      >
    >::other
  // Lastly, n_tree is also a contained_storeT
  , public contained_storeT
{
public:
  /**
   * Convenience typedefs
   **/
  // Allocator for n_tree
  typedef typename allocatorT::template rebind<
    n_tree
  >::other node_allocator_t;

  // The specific child_container we're subclassing
  typedef typename detail::child_container<
    CHILD_NODES,
    n_tree,
    typename allocatorT::template rebind<
      n_tree
    >::other
  > child_container_t;

  typedef child_container_t *  child_container_ptr;

  // Allocator for the child_container_t
  typedef typename allocatorT::template rebind<
    child_container_t
  >::other child_container_allocator_t;

  // Value store related
  typedef contained_storeT contained_store_t;

  BOOST_CLASS_REQUIRE(contained_store_t, fhtagn::containers::concepts,
      ContainedStoreConcept);

  /**
   * Emdedded typedefs for container semantics. This is a container of n_tree,
   * so value_type does *not* represent containedT
   **/
  typedef n_tree                                              value_type;
  typedef n_tree *                                            pointer;
  typedef n_tree const *                                      const_pointer;
  typedef n_tree &                                            reference;
  typedef n_tree const &                                      const_reference;
  typedef typename child_container_t::size_type               size_type;
  typedef typename child_container_t::difference_type         difference_type;

  // FIXME need to wrap iterators. ???
  typedef typename child_container_t::iterator                iterator;
  typedef typename child_container_t::const_iterator          const_iterator;
  typedef typename child_container_t::reverse_iterator        reverse_iterator;
  typedef typename child_container_t::const_reverse_iterator  const_reverse_iterator;

  // FIXME
//      typedef typename _Rep_type::allocator_type         allocator_type;

  /**
   * Embedded typedefs for pointer semantics. Since value_type is already
   * defined by the container semantincs, we'll use a 'contained' prefix.
   **/
  typedef containedT    contained_type;
  typedef containedT *  contained_ptr;


  /***************************************************************************
   * Construction, destruction, assignment.
   **/

  /**
   * Creates an empty n_tree container, i.e. a leaf node.
   **/
  inline n_tree();

  /**
   * The copy constructor creates a deep copy of the n_tree it's passed.
   * FIXME parent
   **/
  inline n_tree(const_reference other);

  // TODO more constructors

  /**
   * Creates a non-leaf n_tree container, initializing it's elements with
   * copies from the range defined by [first, last). The range is copied
   * until either CHILD_NODES or last, is reached, whichever comes first.
   **/
  template <typename input_iterT>
  inline n_tree(input_iterT first, input_iterT last);

  /**
   * Performs a join() prior to clearing up own data members.
   **/
  inline ~n_tree();

  /**
   * First assignment results in a join() of this node and clearing other
   * data members. Then, n_tree performs a deep copy of the n_tree passed to
   * the operator.
   * FIXME parent remains
   **/
  inline reference operator=(const_reference other);


  /***************************************************************************
   * n_tree interface
   **/

  /**
   * Returns true if this node is a leaf node (i.e. has no children), else
   * false. Also @see empty() below.
   **/
  inline bool leaf() const;

  /**
   * Turns a leaf node into a non-leaf node.
   * On leaf nodes, the post condition is that leaf() above returns false.
   * On non-leaf nodes, this is a no-op.
   **/
  inline void split();

  /**
   * Turns a non-leaf node into a leaf node.
   * On non-leaf nodes, the post condition is that leaf() above returns true.
   * On leaf nodes, this is a no-op.
   * Also @see clear() below.
   **/
  inline void join();

  /**
   * Returns a pointer to the parent n_tree, if any exists. For root nodes/
   * unowned nodes, NULL is returned.
   **/
  inline pointer parent() const;

  /**
   * Returns a pointer to the next/previous sibling, or NULL if there is no
   * such sibling.
   **/
  inline pointer next() const;
  inline pointer prev() const;

  /**
   * If this node has a parent, this operation erases this node from the
   * parent's siblings, and sets this node's parent node to NULL, turning
   * it into a new root.
   *
   * Note: this function does *not* delete any elements, just references to
   *       them. That is, the following code snippet leaks a tree_t object:
   *          {
   *            tree_t & t = some_tree[idx];
   *            t.unlink();
   *          }
   *
   *      This can be rectified, however, by manually deleting t:
   *          delete &t;
   **/
  inline void unlink();


  /***************************************************************************
   * Container interface
   **/

  /**
   * Iterate over the range of child nodes.
   *
   * Note: The past-the-end iterator (return value of end() et. al. is not
   * valid when this node is a leaf() node, and results for using it are
   * undefined.
   **/
  inline iterator begin();
  inline const_iterator begin() const;

  inline iterator end();
  inline const_iterator end() const;

  inline reverse_iterator rbegin();
  inline const_reverse_iterator rbegin() const;

  inline reverse_iterator rend();
  inline const_reverse_iterator rend() const;

  /**
   * Returns true if this node is either a leaf node, or is a non-leaf node
   * without children. Also @see leaf() above.
   **/
  inline bool empty() const;

  /**
   * Returns the number of children this node contains.
   **/
  inline size_type size() const;

  /**
   * Returns the number of children this node can contain with the amount of
   * memory currently allocated for them.
   **/
  inline size_type capacity() const;

  /**
   * Returns the maximum number of children this node contains - i.e. the same
   * as CHILD_NODES.
   **/
  inline size_type max_size() const;

  /**
   * Returns the child node at the given index. If the index is larger than
   * CHILD_NODES, std::runtime_error() is thrown.
   *
   * Note #1: The return value is undefined if this is a leaf() node.
   *
   * Note #2: The return value is undefined if the index is out of range.
   *
   * Note #3: For the immutable version, if index references an empty node,
   *  the return value is undefined.
   **/
  inline reference operator[](size_type const & idx);
  inline const_reference operator[](size_type const & idx) const;

  /**
   * Erases child nodes corresponding to the given iterator/value/index.
   *
   * Note that beause erasing does not alter the node's capacity, erasing
   * does not invalidate any iterators.
   **/
  inline void erase(iterator position);
  inline void erase(iterator first, iterator last);
  inline void erase(contained_type const & v);
  inline void erase(value_type const & v);
  inline void erase(size_type const & idx);

  /**
   * Erases all child nodes, but does not turn the node into a leaf node.
   * Also @see join() above.
   **/
  inline void clear();

  /**
   * Find the first node corresponding to the given value.
   **/
  inline iterator find(value_type const & v);
  inline const_iterator find(value_type const & v) const;
  inline iterator find(contained_type const & v);
  inline const_iterator find(contained_type const & v) const;
  inline iterator find(contained_type const & v, iterator start);
  inline const_iterator find(contained_type const & v, const_iterator start) const;

  /**
   * Counts the number of child nodes with the given value.
   **/
  inline size_type count(contained_type const & v) const;

  /**
   * Swaps the value and child nodes of this n_tree with that of another.
   **/
  inline void swap(n_tree & other);


  /***************************************************************************
   * Pointer interface
   **/

  /**
   * Returns a reference to this n_tree's value. If this n_tree does not yet
   * contain a value, this operation creates a default-constructed value_type
   * and returns that.
   **/
  inline contained_type & operator*();
  inline contained_type const & operator*() const;

  /**
   * Returns a pointer to this n_tree's value, and NULL if this n_tree does
   * not contain a value.
   **/
  inline contained_ptr operator->() const;

  /**
   * Same as operator->()
   **/
  inline contained_ptr get() const;

  /**
   * Returns true if this n_tree contains a value, false otherwise.
   **/
  inline operator bool () const;

  /**
   * Redundant, but some compilers need it.
   **/
  inline bool operator!() const;


  /***************************************************************************
   * Comparison operators
   **/

  /**
   * Returns true if a deep comparison between this n_tree and another contains
   * the same value and child values at the same position in the tree.
   **/
  inline bool operator==(n_tree const & other) const;

  /**
   * Returns true if a deep comparison of this n_tree and another yields that
   * this n_tree's value compares less than anothers, has fewer child nodes
   * than another, or it's or child_values compare less than another's child
   * nodes.
   *
   * Note: there's a lot of short-circuit comparison going on there, i.e. a node
   *       with a smaller value will be declared less than another, and their
   *       children won't be compared any more.
   **/
  inline bool operator<(n_tree const & other) const;

  /**
   * Derived comparison operators.
   **/
  inline bool operator!=(n_tree const & other) const;
  inline bool operator>(n_tree const & other) const;
  inline bool operator<=(n_tree const & other) const;
  inline bool operator>=(n_tree const & other) const;

  /**
   * The same thing for contained_type - just compares the value, not the
   * children. If this node has no valid key, == is always false and < is
   * always true.
   **/
  inline bool operator==(contained_type const & other) const;
  inline bool operator<(contained_type const & other) const;

  /**
   * Derived comparison operators.
   **/
  inline bool operator!=(contained_type const & other) const;
  inline bool operator>(contained_type const & other) const;
  inline bool operator<=(contained_type const & other) const;
  inline bool operator>=(contained_type const & other) const;

private:
  typename contained_store_t::key_type  m_key;
  pointer                               m_parent;
  child_container_ptr                   m_children;
};


}} // namespace fhtagn::containers

#include <fhtagn/containers/detail/ntree.tcc>

#endif // guard
