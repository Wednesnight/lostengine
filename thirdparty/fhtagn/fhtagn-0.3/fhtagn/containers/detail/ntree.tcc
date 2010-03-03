/**
 * $Id: ntree.tcc 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_CONTAINERS_DETAIL_NTREE_TCC
#define FHTAGN_CONTAINERS_DETAIL_NTREE_TCC

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <string.h>

#include <stdexcept>
#include <algorithm>
#include <boost/bind.hpp>

namespace fhtagn {
namespace containers {

namespace detail {

/*****************************************************************************
 * default_contained_store implementation
 **/

template <
  typename containedT,
  typename allocatorT
>
struct default_contained_store
{
  /**
   * Convenience typedefs
   **/
  typedef containedT    contained_t;
  typedef containedT *  contained_ptr;

  /**
   * Key type is the same as the pointer here - we don't try to save space,
   * and it makes for the simplest implementation.
   **/
  typedef contained_ptr key_type;

  /**
   * More convenience typededefs
   **/
  typedef std::pair<key_type, contained_ptr>  key_value_t;


  /***************************************************************************
   * API
   **/

  /**
   * Given that we use pointers for keys, it makes sense to use the NULL value
   * as an invalid key.
   **/
  static inline key_type invalid_key()
  {
    return NULL;
  }


  /**
   * Retrieval of values is simple - the key is not just any pointer, it's the
   * pointer to the actual value.
   **/
  inline contained_ptr retrieve(key_type const & key) const
  {
    return key;
  }


  /**
   * Creation should return a key and a pointer, but in our case they're
   * the same.
   **/
  inline key_value_t create()
  {
    contained_ptr x = new contained_t();
    return std::make_pair(x, x);
  }


  /**
   * Copying *can* perform reference counting, in which case the key returned
   * should probably be identical to the key passed to this function.
   **/
  inline key_value_t copy(key_type const & key)
  {
    contained_ptr x = new contained_t(*key);
    return std::make_pair(x, x);
  }


  /**
   * If copying increases a reference count, discarded would decrease it.
   **/
  inline void discard(key_type const & key)
  {
    delete key;
  }
};


/*****************************************************************************
 * Default child_container implementation
 **/
template <
  fhtagn::size_t CHILD_NODES,
  typename nodeT,
  typename allocatorT
>
struct child_container
  : public allocatorT::template rebind<
      nodeT *
    >::other
{
  typedef nodeT    node_t;
  typedef node_t * node_ptr;

  // This is the allocator for this container's value types, i.e. node_ptr and
  // not node_t.
  typedef typename allocatorT::template rebind<node_ptr>::other allocator_t;

  // Embedded typedefs
  typedef typename allocator_t::value_type              value_type;
  typedef typename allocator_t::pointer                 pointer;
  typedef typename allocator_t::const_pointer           const_pointer;
  typedef typename allocator_t::reference               reference;
  typedef typename allocator_t::const_reference         const_reference;
  typedef typename allocator_t::size_type               size_type;
  typedef typename allocator_t::difference_type         difference_type;

  typedef node_ptr *                            iterator;
  typedef node_ptr const *                      const_iterator;
  typedef std::reverse_iterator<iterator>       reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  // Maximum number of children
  enum {
    MAX_SIZE = CHILD_NODES,
  };


  // Vector-like functions - minimal interface for the child container
  inline child_container()
  {
    ::memset(m_data, 0, CHILD_NODES * sizeof(node_ptr));
  }

  inline ~child_container()
  {
    clear();
  }


  inline iterator begin()
  {
    return &m_data[0];
  }

  inline const_iterator begin() const
  {
    return &m_data[0];
  }

  inline iterator end()
  {
    return &m_data[CHILD_NODES];
  }

  inline const_iterator end() const
  {
    return &m_data[CHILD_NODES];
  }

  inline reverse_iterator rbegin()
  {
    return reverse_iterator(end());
  }

  inline const_reverse_iterator rbegin() const
  {
    return const_reverse_iterator(end());
  }

  inline reverse_iterator rend()
  {
    return reverse_iterator(begin());
  }

  inline const_reverse_iterator rend() const
  {
    return const_reverse_iterator(begin());
  }


  inline void resize(size_type, node_ptr const & x = node_ptr())
  {
    // no-op.
  }


  inline bool empty() const
  {
    bool ret = true;
    for (size_type i = 0 ; i < CHILD_NODES ; ++i) {
      if (m_data[i]) {
        ret = false;
        break;
      }
    }
    return ret;
  }



  inline size_type size() const
  {
    size_type count = 0;
    for (size_type i = 0 ; i < CHILD_NODES ; ++i) {
      if (m_data[i]) {
        ++count;
      }
    }
    return count;
  }



  inline size_type capacity() const
  {
    return MAX_SIZE;
  }



  inline void clear()
  {
    for (size_type i = 0 ; i < CHILD_NODES ; ++i) {
      if (m_data[i]) {
        this->allocator_t::destroy(&m_data[i]);
        m_data[i] = NULL;
      }
    }
  }


  inline void erase(iterator iter)
  {
    size_type idx = iter - begin();
    this->allocator_t::destroy(&m_data[idx]);
    m_data[idx] = NULL;
  }


  node_ptr  m_data[CHILD_NODES];
};


/*****************************************************************************
 * child_container specialization for unlimited child nodes.
 **/
template <
  typename nodeT,
  typename allocatorT
>
struct child_container<0, nodeT, allocatorT>
  : public std::vector<
      nodeT *,
      typename allocatorT::template rebind<nodeT *>::other
    >
{
  typedef nodeT    node_t;
  typedef node_t * node_ptr;

  typedef typename std::vector<
    nodeT *,
    typename allocatorT::template rebind<node_ptr>::other
  > vector_t;


  enum {
    // XXX May not be portable; same implementation as max_size() for
    // std::vector on GCC.
    MAX_SIZE = typename vector_t::size_type(-1)
      / sizeof(typename vector_t::value_type),
  };


  typename vector_t::size_type size() const
  {
    return std::count_if(vector_t::begin(), vector_t::end(),
        boost::bind(std::not_equal_to<nodeT *>(), _1,
          static_cast<nodeT *>(NULL)));
  }


  inline void erase(typename vector_t::iterator iter)
  {
    node_ptr p = *iter;
    this->get_allocator().destroy(&p);
    *iter = NULL;
  }
};



/*****************************************************************************
 * Helper functions for copying from iterator range.
 **/
template <
  typename treeT,
  typename inputT
>
inline void
construct_from(treeT & tree, treeT * new_obj, inputT const & value)
{
  typedef typename treeT::node_allocator_t node_allocator_t;
  tree.node_allocator_t::construct(new_obj, treeT());
  *new_obj = *value;
}



// Specializations for constructing from n_tree itself.
template <
  typename treeT
>
inline void
construct_from(treeT & tree, treeT * new_obj, treeT const * value)
{
  typedef typename treeT::node_allocator_t node_allocator_t;
  tree.node_allocator_t::construct(new_obj, *value);
}



template <
  typename treeT
>
inline void
construct_from(treeT & tree, treeT * new_obj, treeT * value)
{
  typedef typename treeT::node_allocator_t node_allocator_t;
  tree.node_allocator_t::construct(new_obj, *value);
}



template <
  typename treeT
>
inline void
construct_from(treeT & tree, treeT * new_obj, treeT const & value)
{
  typedef typename treeT::node_allocator_t node_allocator_t;
  tree.node_allocator_t::construct(new_obj, value);
}




template <
  typename treeT,
  typename valueT
>
inline bool
is_null_n_tree(valueT const &)
{
  return false;
}


template <
  typename treeT
>
inline bool
is_null_n_tree(treeT * t)
{
  return (NULL == t);
}

} // namespace detail



/*****************************************************************************
 * n_tree implementation - construction, destruction, assignment
 */
template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::n_tree()
  : contained_store_t()
  , m_key(contained_store_t::invalid_key())
  , m_parent(NULL)
  , m_children(NULL)
{
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::n_tree(
    const_reference other)
  : contained_store_t(other)
  , m_key(contained_store_t::invalid_key())
  , m_parent(NULL)
  , m_children(NULL)
{
  // All the copying is handled by the assignment operator - that's why the
  // initializer list above is identical to that of the default ctor.
  this->operator=(other);
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
template <typename input_iterT>
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::n_tree(
    input_iterT first, input_iterT last)
  : contained_store_t()
  , m_key(contained_store_t::invalid_key())
  , m_parent(NULL)
  , m_children(NULL)
{
  // Iterators need to be in the right order.
  size_type input_size = size_type(last - first);
  if (input_size <= 0) {
    return;
  }

  split();

  // Allocate enough capacity. For unlimited trees, that's exactly the size
  // of the input range, but for limited trees that may not be the case.
  if (CHILD_NODES) {
    input_size = std::min<size_type>(CHILD_NODES, input_size);
  }
  m_children->resize(input_size, NULL);

  // Now copy from the input range.
  size_type idx = 0;
  for (input_iterT input_iter = first ; idx != input_size ; ++input_iter, ++idx) {
    // If input_iterT is a n_tree-iterator of any type, it points to a n_tree *,
    // which can be NULL - we'll want to skip in that case.
    if (detail::is_null_n_tree<n_tree>(*input_iter)) {
      continue;
    }

    // Copy construct from input element - avoids a default construction
    // and subsequent assignment scenario
    iterator iter = m_children->begin() + idx;
    *iter = this->node_allocator_t::allocate(1);
    if (!*iter) {
      throw std::bad_alloc();
    }
    detail::construct_from(*this, *iter, *input_iter);
    (*iter)->m_parent = this;
  }
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::~n_tree()
{
  // The join function conveniently destroys all children - we might as well
  // reuse it here.
  join();
  unlink();
  if (contained_store_t::invalid_key() != m_key) {
    contained_store_t::discard(m_key);
  }
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
typename n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::reference
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::operator=(
    const_reference other)
{
  // Short-circuit
  if (this == &other) {
    return *this;
  }

  // Clear out this node. These functions only consume time if there's anything
  // to do.
  join();
  if (contained_store_t::invalid_key() != m_key) {
    contained_store_t::discard(m_key);
    m_key = contained_store_t::invalid_key();
  }

  // We copy the other node's key, and we copy the other node's children. We
  // do *not* copy the other node's parent, as that'd destroy the parent-/child-
  // relationships.
  if (contained_store_t::invalid_key() != other.m_key) {
    typename contained_store_t::key_value_t kv = contained_store_t::copy(
        other.m_key);
    m_key = kv.first;
  }

  if (other.m_children) {
    // Allocate and construct child container
    split();
    if (m_children->capacity() < other.m_children->capacity()) {
      m_children->resize(other.m_children->capacity(), NULL);
    }

    // Now copy elements from other over.
    typename child_container_t::size_type index = 0;
    typename child_container_t::const_iterator child_end = other.m_children->end();
    for (typename child_container_t::const_iterator child_iter = other.m_children->begin()
        ; child_iter != child_end ; ++child_iter, ++index)
    {
      if (!*child_iter) {
        continue;
      }

      n_tree * n = *child_iter;
      if (!n) {
        continue;
      }

      // Copy construct from other's element - avoids a default construction
      // and subsequent assignment scenario
      iterator iter = m_children->begin() + index;
      *iter = this->node_allocator_t::allocate(1);
      if (!*iter) {
        throw std::bad_alloc();
      }
      this->node_allocator_t::construct(*iter, *n);
      (*iter)->m_parent = this;
    }
  }

  return *this;
}


/*****************************************************************************
 * n_tree implementation
 */
template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
bool
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::leaf() const
{
  return !m_children;
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
void
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::split()
{
  if (m_children) {
    return;
  }

  // Allocate and construct child container
  m_children = this->child_container_allocator_t::allocate(1);
  if (!m_children) {
    throw std::bad_alloc();
  }
  this->child_container_allocator_t::construct(m_children, child_container_t());
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
void
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::join()
{
  if (!m_children) {
    return;
  }

  clear();

  // Destroy child container itself.
  this->child_container_allocator_t::destroy(m_children);
  this->child_container_allocator_t::deallocate(m_children, 1);
  m_children = NULL;
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
typename n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::pointer
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::parent() const
{
  return m_parent;
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
typename n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::pointer
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::next() const
{
  if (!m_parent || !m_parent->m_children) {
    return NULL;
  }

  iterator iter = m_parent->find(*this);
  ++iter;
  if (m_parent->end() == iter) {
    return NULL;
  }
  return *iter;
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
typename n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::pointer
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::prev() const
{
  if (!m_parent || !m_parent->m_children) {
    return NULL;
  }


  iterator iter = m_parent->find(*this);
  --iter;
  if (m_parent->end() == iter) {
    return NULL;
  }
  return *iter;
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
void
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::unlink()
{
  if (!m_parent) {
    return;
  }

  if (!m_parent->m_children) {
    throw std::logic_error("Node professes to be the child of a leaf node.");
  }

  iterator iter = m_parent->find(*this);
  if (m_parent->end() == iter) {
    throw std::logic_error("Node professes to be the child of a parent node "
        "that doesn't recognize it.");
  }

  m_parent->m_children->erase(iter);
  m_parent = NULL;
}


/*****************************************************************************
 * n_tree container interface implementation
 **/

template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
typename n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::iterator
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::begin()
{
  if (!m_children) {
    return end();
  }
  return m_children->begin();
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
typename n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::const_iterator
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::begin() const
{
  if (!m_children) {
    return end();
  }
  return m_children->begin();
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
typename n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::iterator
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::end()
{
  if (!m_children) {
    return typename child_container_t::iterator();
  }
  return m_children->end();
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
typename n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::const_iterator
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::end() const
{
  if (!m_children) {
    return typename child_container_t::const_iterator();
  }
  return m_children->end();
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
typename n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::reverse_iterator
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::rbegin()
{
  if (!m_children) {
    return rend();
  }
  return m_children->rbegin();
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
typename n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::const_reverse_iterator
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::rbegin() const
{
  if (!m_children) {
    return rend();
  }
  return m_children->rbegin();
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
typename n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::reverse_iterator
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::rend()
{
  if (!m_children) {
    return typename child_container_t::reverse_iterator();
  }
  return m_children->rend();
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
typename n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::const_reverse_iterator
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::rend() const
{
  if (!m_children) {
    return typename child_container_t::const_reverse_iterator();
  }
  return m_children->rend();
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
bool
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::empty() const
{
  if (!m_children) {
    return true;
  }
  return m_children->empty();
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
typename n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::size_type
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::size() const
{
  if (!m_children) {
    return size_type(0);
  }
  return m_children->size();
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
typename n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::size_type
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::capacity() const
{
  if (!m_children) {
    return size_type(0);
  }
  return m_children->capacity();
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
typename n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::size_type
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::max_size() const
{
  return child_container_t::MAX_SIZE;
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
typename n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::reference
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::operator[](size_type const & idx)
{
  if (!m_children) {
    static n_tree * dummy = NULL;
    return *dummy; // undefined behaviour, as documented
  }

  // If necessaroy, grow the child container. Pretty much a no-op in most cases.
  if (m_children->capacity() < idx + 1) {
    m_children->resize(idx + 1, NULL);
  }

  // Find the specified child.
  iterator iter = m_children->begin();
  iter += idx;

  // If there is no child at the given index, create one now.
  if (!*iter) {
    *iter = this->node_allocator_t::allocate(1);
    if (!*iter) {
      throw std::bad_alloc();
    }
    this->node_allocator_t::construct(*iter, n_tree());
    (*iter)->m_parent = this;
  }

  return **iter;
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
typename n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::const_reference
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::operator[](size_type const & idx) const
{
  static n_tree * dummy = NULL;
  if (!m_children || m_children->capacity() <= idx) {
    return *dummy; // undefined behaviour, as documented
  }

  const_iterator iter = m_children->begin();
  iter += idx;
  if (!*iter) {
    return *dummy; // undefined behaviour, as documented
  }

  return **iter;
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
void
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::erase(
    iterator position)
{
  if (position == end()) {
    return;
  }

  if (!*position) {
    return;
  }

  if (!m_children) {
    return;
  }

  // XXX Safety check, not strictly required.
  size_type idx = position - m_children->begin();
  if (0 > idx || idx >= m_children->capacity()) {
    // Not a valid iterator!
    return;
  }

  n_tree * n = *position;
  this->node_allocator_t::destroy(n);
  this->node_allocator_t::deallocate(n, 1);

  m_children->erase(position);
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
void
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::erase(
    iterator first, iterator last)
{
  if (!m_children) {
    return;
  }

  for (typename child_container_t::iterator iter = first ; iter != last ; ++iter)
  {
    if (!*iter) {
      continue;
    }

    n_tree * n = *iter;
    if (!n) {
      continue;
    }

    this->node_allocator_t::destroy(n);
    this->node_allocator_t::deallocate(n, 1);

    m_children->erase(iter);
  }
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
void
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::erase(
    contained_type const & v)
{
  if (!m_children) {
    return;
  }

  typename child_container_t::iterator end = m_children->end();
  for (typename child_container_t::iterator iter = m_children->begin()
      ; iter != end ; ++iter)
  {
    if (!*iter) {
      continue;
    }

    n_tree * n = *iter;
    if (!n) {
      continue;
    }

    if (v != **n) {
      continue;
    }


    this->node_allocator_t::destroy(n);
    this->node_allocator_t::deallocate(n, 1);

    m_children->erase(iter);
  }
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
void
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::erase(
    value_type const & v)
{
  erase(find(v));
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
void
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::erase(size_type const & idx)
{
  if (!m_children || m_children->capacity() <= idx) {
    return;
  }

  typename child_container_t::iterator iter = m_children->begin();
  iter += idx;

  n_tree * n = *iter;
  this->node_allocator_t::destroy(n);
  this->node_allocator_t::deallocate(n, 1);

  m_children->erase(iter);
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
void
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::clear()
{
  if (!m_children) {
    return;
  }

  // The child container is a container of child pointers only - we need
  // to destroy the child objects manually.
  typename child_container_t::const_iterator child_end = m_children->end();
  for (typename child_container_t::const_iterator child_iter = m_children->begin()
      ; child_iter != child_end ; ++child_iter)
  {
    if (*child_iter) {
      // Destroy and deallocate child; note that this does not destroy the child
      // pointer in the container.
      n_tree * n = *child_iter;
      this->node_allocator_t::destroy(n);
      this->node_allocator_t::deallocate(n, 1);
    }
  }

  // Destroy all pointers to child nodes in the container.
  m_children->clear();
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
typename n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::iterator
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::find(
    value_type const & v)
{
  if (!m_children) {
    return end();
  }

  typename child_container_t::iterator child_end = m_children->end();
  for (typename child_container_t::iterator iter = m_children->begin()
      ; iter != child_end ; ++iter)
  {
    if (!*iter) {
      continue;
    }

    n_tree * n = *iter;
    if (!n) {
      continue;
    }

    if (v == *n) {
      return iter;
    }
  }

  return end();
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
typename n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::const_iterator
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::find(
    value_type const & v) const
{
  if (!m_children) {
    return end();
  }

  typename child_container_t::const_iterator child_end = m_children->end();
  for (typename child_container_t::const_iterator iter = m_children->begin()
      ; iter != child_end ; ++iter)
  {
    if (!*iter) {
      continue;
    }

    n_tree * n = *iter;
    if (!n) {
      continue;
    }

    if (v == *n) {
      return iter;
    }
  }

  return end();
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
typename n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::iterator
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::find(
    contained_type const & v)
{
  return find(v, begin());
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
typename n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::const_iterator
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::find(
    contained_type const & v) const
{
  return find(v, begin());
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
typename n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::iterator
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::find(
    contained_type const & v, iterator start)
{
  if (!m_children) {
    return end();
  }

  typename child_container_t::iterator child_end = m_children->end();
  for (typename child_container_t::iterator iter = start
      ; iter != child_end ; ++iter)
  {
    if (!*iter) {
      continue;
    }

    n_tree * n = *iter;
    if (!n) {
      continue;
    }

    if (v == **n) {
      return iter;
    }

  }

  return end();
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
typename n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::const_iterator
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::find(
    contained_type const & v, const_iterator start) const
{
  if (!m_children) {
    return end();
  }

  typename child_container_t::const_iterator child_end = m_children->end();
  for (typename child_container_t::const_iterator iter = start
      ; iter != child_end ; ++iter)
  {
    if (!*iter) {
      continue;
    }

    n_tree * n = *iter;
    if (!n) {
      continue;
    }

    if (v == **n) {
      return iter;
    }
  }

  return end();
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
typename n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::size_type
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::count(
    contained_type const & v) const
{
  size_type ret = 0;

  const_iterator child_end = end();
  const_iterator iter = find(v);
  while (iter != child_end) {
    ++ret;
    iter = find(v, ++iter);
  }

  return ret;
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
void
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::swap(n_tree & other)
{
  std::swap(m_key, other.m_key);
  std::swap(m_parent, other.m_parent);
  std::swap(m_children, other.m_children);
}



/*****************************************************************************
 * n_tree pointer interface implementation
 **/
template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
typename n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::contained_type &
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::operator*()
{
  contained_ptr value = contained_store_t::retrieve(m_key);
  if (value) {
    return *value;
  }

  typename contained_store_t::key_value_t new_value = contained_store_t::create();
  m_key = new_value.first;
  return *new_value.second;
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
typename n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::contained_type const &
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::operator*() const
{
  contained_ptr value = contained_store_t::retrieve(m_key);
  return *value;
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
typename n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::contained_ptr
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::operator->() const
{
  contained_ptr value = contained_store_t::retrieve(m_key);
  if (value) {
    return value;
  }

  return NULL;
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
typename n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::contained_ptr
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::get() const
{
  return contained_store_t::retrieve(m_key);
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::operator bool () const
{
  return !!contained_store_t::retrieve(m_key);
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
bool
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::operator!() const
{
  return !contained_store_t::retrieve(m_key);
}


/*****************************************************************************
 * n_tree implementation - comparison operators
 **/

template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
bool
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::operator==(
    n_tree const & other) const
{
  // Short-circuit
  if (this == &other) {
    return true;
  }

  // If this or other has no key, it has no value, and therefore is smaller.
  if (contained_store_t::invalid_key() == m_key
      && other.contained_store_t::invalid_key() != other.m_key)
  {
    return false;
  }
  if (contained_store_t::invalid_key() != m_key
      && other.contained_store_t::invalid_key() == other.m_key)
  {
    return false;
  }

  // Now to compare the values. If they don't compare equal, we can already
  // consider the two nodes not to be equal.
  if (contained_store_t::invalid_key() != m_key) {
    contained_ptr this_value = contained_store_t::retrieve(m_key);
    contained_ptr other_value = other.contained_store_t::retrieve(
        other.m_key);
    if (*this_value != *other_value) {
      return false;
    }
  }

  // Either both nodes have children, nor neither have children, but there
  // can't be imbalance.
  if ((m_children && !other.m_children)
      || (!m_children && other.m_children))
  {
    return false;
  }

  // Now that we know there's no imbalance between children, if this node has
  // none, then both nodes must be equal.
  if (!m_children) {
    return true;
  }

  // Special case for nodes with CHILD_NODES == 0: do the child containers have
  // the same size?
  if (m_children->size() != other.m_children->size()) {
    return false;
  }

  // If this node has children, then the other does, and both nodes' children
  // must be compared.
  typename child_container_t::const_iterator this_end = m_children->end();
  typename child_container_t::const_iterator this_iter = m_children->begin();
  typename child_container_t::const_iterator other_iter = other.m_children->begin();
  for (; this_iter != this_end ; ++this_iter, ++other_iter) {
    // Each slot must either have a child in both nodes, or none, but not mixed.
    if ((*this_iter && !*other_iter)
        || (!*this_iter && *other_iter))
    {
      return false;
    }

    if (!*this_iter) {
      continue;
    }

    n_tree * this_child = *this_iter;
    n_tree * other_child = *other_iter;
    if (*this_child != *other_child) {
      return false;
    }
  }

  // No other option now.
  return true;
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
bool
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::operator<(
    n_tree const & other) const
{
  // Short-circuit
  if (this == &other) {
    return false;
  }

  // If this or other has no key, it has no value, and therefore is smaller.
  if (contained_store_t::invalid_key() == m_key
      && other.contained_store_t::invalid_key() != other.m_key)
  {
    return true;
  }
  if (contained_store_t::invalid_key() != m_key
      && other.contained_store_t::invalid_key() == other.m_key)
  {
    return false;
  }

  // We must now retrieve the values of both this value and the other value.
  // Only if they're equal do we continue.
  contained_ptr this_value = contained_store_t::retrieve(m_key);
  contained_ptr other_value = other.contained_store_t::retrieve(
      other.m_key);
  if (*this_value < *other_value) {
    return true;
  }
  if (*this_value > *other_value) {
    return false;
  }

  // We must compare children next. A node without children is less than a node
  // with children.
  if (!m_children && other.m_children) {
    return true;
  }
  if (m_children && !other.m_children) {
    return false;
  }

  // Now that we know there's no imbalance between children, if this node has
  // none, then both nodes must be equal.
  if (!m_children) {
    return false;
  }

  // Special case for nodes with CHILD_NODES == 0: do the child containers have
  // the same size?
  if (m_children->size() < other.m_children->size()) {
    return true;
  }
  if (m_children->size() > other.m_children->size()) {
    return false;
  }

  // If this node has children, then the other does, and both nodes' children
  // must be compared.
  typename child_container_t::const_iterator this_end = m_children->end();
  typename child_container_t::const_iterator this_iter = m_children->begin();
  typename child_container_t::const_iterator other_iter = other.m_children->begin();
  for (; this_iter != this_end ; ++this_iter, ++other_iter) {
    // Each slot must either have a child in both nodes, or none, but not mixed.
    if (!*this_iter && *other_iter) {
      return true;
    }
    if (*this_iter && !*other_iter) {
      return false;
    }

    if (!*this_iter) {
      continue;
    }

    n_tree * this_child = *this_iter;
    n_tree * other_child = *other_iter;
    if (*this_child < *other_child) {
      return true;
    }
    if (*this_child > *other_child) {
      return false;
    }
  }

  // No other option now, the nodes are equal.
  return false;
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
bool
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::operator!=(
    n_tree const & other) const
{
  return !this->operator==(other);
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
bool
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::operator>(
    n_tree const & other) const
{
  return !this->operator<=(other);
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
bool
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::operator<=(
    n_tree const & other) const
{
  return (this->operator==(other) || this->operator<(other));
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
bool
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::operator>=(
    n_tree const & other) const
{
  return !this->operator<(other);
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
bool
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::operator==(
    contained_type const & other) const
{
  if (contained_store_t::invalid_key() == m_key) {
    return false;
  }

  contained_ptr value = contained_store_t::retrieve(m_key);
  if (!value) {
    return false;
  }

  return (*value == other);
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
bool
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::operator<(
    contained_type const & other) const
{
  if (contained_store_t::invalid_key() == m_key) {
    return true;
  }

  contained_ptr value = contained_store_t::retrieve(m_key);
  if (!value) {
    return true;
  }

  return (*value < other);
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
bool
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::operator!=(
    contained_type const & other) const
{
  return !this->operator==(other);
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
bool
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::operator>(
    contained_type const & other) const
{
  return !this->operator<=(other);
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
bool
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::operator<=(
    contained_type const & other) const
{
  return (this->operator==(other) || this->operator<(other));
}



template <
  fhtagn::size_t CHILD_NODES,
  typename containedT,
  typename allocatorT,
  typename contained_storeT
>
bool
n_tree<CHILD_NODES, containedT, allocatorT, contained_storeT>::operator>=(
    contained_type const & other) const
{
  return !this->operator<(other);
}



}} // namespace fhtagn::containers

#endif // guard
